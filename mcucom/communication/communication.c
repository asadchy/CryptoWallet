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
#include "communication.h"
#include "i2c.h"
#include "data.hpp"
#include "string.h"

SemaphoreHandle_t packet_ready_sem;
struct ipc_packet tx_packet, rx_packet;
struct ipc_wrapper wrp;

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

	BOARD_InitI2C(&wrp);

	while(1)
	{
		//send packet to PN
		if(uxQueueMessagesWaiting(lpc_to_pn) /*&& xSemaphoreTake(packet_ready_sem, 0)*/)	//нужен ли семафор в этом случае?
		{
			if(xQueueReceive(lpc_to_pn, (void*)&(tx_packet.data), 0))
			{
				tx_packet.ready = true;
			}
		}
		//receive packet from PN
		if(rx_packet.ready /*&& xSemaphoreTake(packet_ready_sem, 0)*/)		//нужен ли семафор в этом случае?
		{
			if(xQueueSend(pn_to_lpc, (void*)&(rx_packet.data), 0))
			{
				rx_packet.ready = false;
			}
		}
	}
}
