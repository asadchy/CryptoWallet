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
	PINCODE = 0x00,
	TRANSACTION,
	TO_MAIN,
	INIT_PINCODE,
	WRONG_PINCODE,
	TO_STATUS,
	TRANSACTION_CANCELED,
	TRANSACTION_CONFIRMED,
	BLOCKED
};

struct message
{
	int cmd;
	void *data;
};

struct transaction
{
	char curr_name[16];				//currency name
	char addr[43];					//address (ASCII), 42 bytes address + 1 zero terminator
	double value;					//transfer amount
};

#ifndef SIMULATOR
	extern QueueHandle_t lcd_to_card, card_to_lcd;
#endif

#endif /* DATA_HPP_ */
