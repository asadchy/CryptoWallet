/*
 * data.h
 *
 *  Created on: Feb 20, 2018
 *      Author: artyom
 */

#ifndef DATA_HPP_
#define DATA_HPP_

#ifndef SIMULATOR
	#include "FreeRTOS.h"
	#include "task.h"
	#include "queue.h"
#endif

#define QUEUE_SIZE								5

enum COMMANDS
{
	WALLET_PINCODE = 0x00,
	WALLET_MNEMONIC,
	TRANSACTION,
	INIT_PINCODE,
	TRANSACTION_CANCELED,
	TRANSACTION_CONFIRMED,
	WALLET_STATUS,
	WALLET_INIT,
	WALLET_ENTER_PIN,
	WALLET_SET_PIN,
	WALLET_ENTER_MS,
	WALLET_SET_MS,
	WALLET_TRANSACTION,
	WALLET_CMD_INIT,
	WALLET_CMD_RESTORE,
	WALLET_CMD_CLEAR,
	WALLET_CONFIRM_PRESSED,
	WALLET_CANCEL_PRESSED,
	WALLET_WRONG_PINCODE,
	WALLET_PIN_NOT_MATCH,
	WALLET_MS_ACCEPTED,
	BLOCKED
};

struct message
{
	int cmd;
	void *data;
};

enum CURRENCIES
{
	BTC,
	ETH,
	LTC,
	CURR_NUM
};

struct transaction
{
	enum CURRENCIES curr_name;		//currency name
	char addr[43];					//address (ASCIIZ), 42 bytes address + 1 zero terminator
	char value[16];					//transfer amount
};

extern const char * const currencies_list[];

struct currency_status
{
	enum CURRENCIES curr_name;		//currency mnemonic
	char amount[16];				//currency amount (ASCIIZ)
	char amount_dollars[16];		//currency amount in dollars (ASCIIZ)
};

struct wallet_status
{
	struct currency_status curr[CURR_NUM];	//list of available currencies
	int num;								//number of available currencies
};


#ifndef SIMULATOR
	extern QueueHandle_t lcd_to_card, card_to_lcd;
#endif

#endif /* DATA_HPP_ */
