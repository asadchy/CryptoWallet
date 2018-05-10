/*
 * data.c
 *
 *  Created on: Feb 20, 2018
 *      Author: artyom
 */

#include <data.hpp>

#ifndef SIMULATOR
	QueueHandle_t lcd_to_card, card_to_lcd;
	QueueHandle_t uart_rx, uart_tx;
#endif

const char * const currencies_list[CURR_NUM] = {
		"Bitcoin",
		"Ethereum",
		"Litecoin"
};
