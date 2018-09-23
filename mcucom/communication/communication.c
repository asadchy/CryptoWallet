/*
 * communication.c
 *
 *  Created on: Apr 22, 2018
 *      Author: artyom
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stdint.h"
#include "stdio.h"
#include "communication.h"
#include "i2c.h"
#include "data.hpp"
#include "string.h"

SemaphoreHandle_t packet_ready_sem;
struct ipc_packet tx_packet, rx_packet;
struct ipc_wrapper wrp;

static struct message mess_to_lcd, mess_from_lcd;
uint8_t status = 0xFF;
uint32_t pin = 0xFFFFFFFF;
uint32_t attempts_left;
static struct wallet_status currency_info;
static struct transaction trans;

void packet_parser();

void Communication_Task(void *params)
{
	packet_ready_sem = xSemaphoreCreateBinary();
	if(packet_ready_sem == NULL)
	{
		while(1);
	}

	lpc_to_pn = xQueueCreate(COM_QUEUE_SIZE, IPC_BUF_SIZE);
	if(lpc_to_pn == 0)
	{
		while(1);
	}

	pn_to_lpc = xQueueCreate(COM_QUEUE_SIZE, IPC_BUF_SIZE);
	if(pn_to_lpc == 0)
	{
		while(1);
	}

	tx_packet.size = IPC_BUF_SIZE;
	tx_packet.ready = false;

	rx_packet.size = IPC_BUF_SIZE;
	rx_packet.ready = false;

	memset(tx_packet.data, 0, sizeof(tx_packet.data));
	memset(rx_packet.data, 0, sizeof(rx_packet.data));

	wrp.tx = &tx_packet;
	wrp.rx = &rx_packet;
	wrp.sem = &packet_ready_sem;

	currency_info.curr[0].curr_name = BTC;
	currency_info.curr[0].amount_dollars[0] = '0';
	currency_info.curr[0].amount_dollars[1] = 0;
	currency_info.curr[0].amount[0] = '0';
	currency_info.curr[0].amount[1] = 0;

	currency_info.curr[1].curr_name = ETH;
	currency_info.curr[1].amount_dollars[0] = '0';
	currency_info.curr[1].amount_dollars[1] = 0;
	currency_info.curr[1].amount[0] = '0';
	currency_info.curr[1].amount[1] = 0;

	currency_info.curr[2].curr_name = LTC;
	currency_info.curr[2].amount_dollars[0] = '0';
	currency_info.curr[2].amount_dollars[1] = 0;
	currency_info.curr[2].amount[0] = '0';
	currency_info.curr[2].amount[1] = 0;

	trans.curr_name = BTC;
	memcpy(trans.addr, "0x123456789ABCDEF", sizeof("0x123456789ABCDEF"));
	memcpy(trans.value, "100.500", sizeof("100.500"));

	currency_info.num = 3;

	BOARD_InitI2C(&wrp);

	while(1)
	{
		//receive packet from PN
		if(rx_packet.ready /*&& xSemaphoreTake(packet_ready_sem, 0)*/)		//нужен ли семафор в этом случае?
		{
			packet_parser();
			rx_packet.ready = false;
		}

		if(xQueueReceive(lcd_to_card, (void* )&mess_from_lcd, 0))
		{
			int cmd = mess_from_lcd.cmd;
			switch(cmd)
			{
			case WALLET_PINCODE:
				pin = *(uint32_t*)mess_from_lcd.data;
				break;
			}
		}
	}
}

#define POLY 0x8408
static unsigned short crc16(unsigned char *data_p, unsigned short length)
{
      unsigned char i;
      unsigned int data;
      unsigned int crc = 0xffff;

      if (length == 0)
            return (~crc);

      do
      {
            for (i=0, data=(unsigned int)0xff & *data_p++;
                 i < 8;
                 i++, data >>= 1)
            {
                  if ((crc & 0x0001) ^ (data & 0x0001))
                        crc = (crc >> 1) ^ POLY;
                  else  crc >>= 1;
            }
      } while (--length);

      crc = ~crc;
      data = crc;
      crc = (crc << 8) | (data >> 8 & 0xff);

      return (crc);
}

void packet_parser()
{
	if(rx_packet.data[0] == 0x12)
	{
		uint32_t crypto, crypto_fract;
		uint32_t data_len = rx_packet.data[2];
		uint32_t data_header_len = data_len + 3;
		uint16_t calc_crc, rx_crc = (((uint16_t)rx_packet.data[data_header_len]) << 8) | rx_packet.data[data_header_len + 1];

		calc_crc = crc16(rx_packet.data, data_header_len);

		if(calc_crc == rx_crc)
		{
			uint8_t cmd = rx_packet.data[1];
			uint16_t tx_crc;
			switch(cmd)
			{
			case WALLET_ENTER_PIN:
			case WALLET_SET_PIN:
			case WALLET_ENTER_MS:
			case WALLET_SET_MS:
			case WALLET_INIT:
			case WALLET_BLOCKED:
				mess_to_lcd.cmd = cmd;
				xQueueSend(card_to_lcd, (void* )&mess_to_lcd, 0);

				memset(tx_packet.data, 0, sizeof(tx_packet.data));
				tx_packet.data[0] = 0x21;
				tx_packet.data[1] = cmd;
				tx_packet.data[2] = 0x00;
				tx_crc = crc16(tx_packet.data, tx_packet.data[2] + 3);
				tx_packet.data[3] = tx_crc >> 8;
				tx_packet.data[4] = tx_crc;

				tx_packet.ready = true;
				break;

			case WALLET_STATUS:
			case TRANSACTION_CONFIRMED:
				mess_to_lcd.cmd = cmd;
				mess_to_lcd.data = &currency_info;
				xQueueSend(card_to_lcd, (void* )&mess_to_lcd, 0);

				memset(tx_packet.data, 0, sizeof(tx_packet.data));
				tx_packet.data[0] = 0x21;
				tx_packet.data[1] = cmd;
				tx_packet.data[2] = 0x00;
				tx_crc = crc16(tx_packet.data, tx_packet.data[2] + 3);
				tx_packet.data[3] = tx_crc >> 8;
				tx_packet.data[4] = tx_crc;

				tx_packet.ready = true;
				break;

			case WALLET_WRONG_PINCODE:
				mess_to_lcd.cmd = WALLET_WRONG_PINCODE;
				attempts_left = rx_packet.data[3];
				mess_to_lcd.data = &attempts_left;
				xQueueSend(card_to_lcd, (void* )&mess_to_lcd, 0);

				memset(tx_packet.data, 0, sizeof(tx_packet.data));
				tx_packet.data[0] = 0x21;
				tx_packet.data[1] = cmd;
				tx_packet.data[2] = 0x00;
				tx_crc = crc16(tx_packet.data, tx_packet.data[2] + 3);
				tx_packet.data[3] = tx_crc >> 8;
				tx_packet.data[4] = tx_crc;

				tx_packet.ready = true;
				break;

			case WALLET_CURRENCY_INFO:
				;
				uint8_t arg = rx_packet.data[3];
				crypto = ((uint32_t)rx_packet.data[4] << 24) | ((uint32_t)rx_packet.data[5] << 16) |
						((uint32_t)rx_packet.data[6] << 8) | ((uint32_t)rx_packet.data[7]);
				crypto_fract = ((uint32_t)rx_packet.data[8] << 24) | ((uint32_t)rx_packet.data[9] << 16) |
						((uint32_t)rx_packet.data[10] << 8) | ((uint32_t)rx_packet.data[11]);
				uint32_t dollars = ((uint32_t)rx_packet.data[12] << 24) | ((uint32_t)rx_packet.data[13] << 16) |
						((uint32_t)rx_packet.data[14] << 8) | ((uint32_t)rx_packet.data[15]);
				uint32_t dollars_fract = ((uint32_t)rx_packet.data[16] << 24) | ((uint32_t)rx_packet.data[17] << 16) |
						((uint32_t)rx_packet.data[18] << 8) | ((uint32_t)rx_packet.data[19]);
				/* fill struct wallet_status currency_info */
				currency_info.curr[arg].curr_name = arg;
				snprintf(currency_info.curr[arg].amount, 16, "%u.%u", crypto, crypto_fract);
				snprintf(currency_info.curr[arg].amount_dollars, 16, "%u.%u", dollars, dollars_fract);
				memset(tx_packet.data, 0, sizeof(tx_packet.data));
				tx_packet.data[0] = 0x21;
				tx_packet.data[1] = cmd;
				tx_packet.data[2] = 0x01;
				tx_packet.data[2] = arg;
				tx_crc = crc16(tx_packet.data, tx_packet.data[2] + 3);
				tx_packet.data[4] = tx_crc >> 8;
				tx_packet.data[5] = tx_crc;

				tx_packet.ready = true;
				break;

			case WALLET_TRANSACTION:
				/*
				trans.curr_name = rx_packet.data[3];
				memcpy(trans.addr, &rx_packet.data[4], 43);
				crypto = ((uint32_t)rx_packet.data[47] << 24) | ((uint32_t)rx_packet.data[48] << 16) |
						((uint32_t)rx_packet.data[49] << 8) | ((uint32_t)rx_packet.data[50]);
				crypto_fract = ((uint32_t)rx_packet.data[51] << 24) | ((uint32_t)rx_packet.data[52] << 16) |
						((uint32_t)rx_packet.data[53] << 8) | ((uint32_t)rx_packet.data[54]);
				snprintf(trans.value, 16, "%u.%u", crypto, crypto_fract);
				*/
				mess_to_lcd.cmd = WALLET_TRANSACTION;
				mess_to_lcd.data = &trans;
				xQueueSend(card_to_lcd, (void* )&mess_to_lcd, 0);

				memset(tx_packet.data, 0, sizeof(tx_packet.data));
				tx_packet.data[0] = 0x21;
				tx_packet.data[1] = cmd;
				tx_packet.data[2] = 0x00;
				tx_crc = crc16(tx_packet.data, tx_packet.data[2] + 3);
				tx_packet.data[3] = tx_crc >> 8;
				tx_packet.data[4] = tx_crc;

				tx_packet.ready = true;
				break;
			}
		}
	}
}
