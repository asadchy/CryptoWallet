/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.8.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * This file is free software and is provided for example purposes. You may
 * use, copy, and modify within the terms and conditions of the license
 * agreement.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <string.h>

#ifndef SIMULATOR
	#include "data.hpp"
	#include "FreeRTOS.h"
	#include "task.h"
	#include "queue.h"
#endif

Model::Model() : modelListener(0)
{
}

void Model::tick()
{
#ifndef SIMULATOR
	struct message rx_mess;
	if(xQueueReceive(card_to_lcd, static_cast<void*>(&rx_mess), 0))
	{
		message_parser(&rx_mess);
	}
//
//	static int i = 0;
//	if(i == 0)
//	{
//		tx_mess.cmd = WALLET_ENTER_MS;
//		xQueueSend(card_to_lcd, static_cast<void*>(&tx_mess), 0);
//	}
//	else if(i == 300)
//	{
//		tx_mess.cmd = WALLET_ENTER_PIN;
//		xQueueSend(card_to_lcd, static_cast<void*>(&tx_mess), 0);
//	}
//	else if(i == 600)
//	{
//		tx_mess.cmd = WALLET_SET_MS;
//		xQueueSend(card_to_lcd, static_cast<void*>(&tx_mess), 0);
//	}
//	else if(i == 900)
//	{
//		tx_mess.cmd = WALLET_SET_PIN;
//		xQueueSend(card_to_lcd, static_cast<void*>(&tx_mess), 0);
//	}
//	i++;

#endif

#ifdef SIMULATOR
	static int const mul = 1;
	static int tick_counter = 0;
	if(tick_counter == mul * 100)
	{
		toInitScreen();
	}
	else if(tick_counter == mul * 200)
	{
		toPinScreen();
	}
	else if(tick_counter == mul * 300)
	{
		//showMSKeyboard();
		static const char *message = "Enter Your Mnemonic Seed";
		touchgfx::Unicode::strncpy(tmpText, message, TEXT_SIZE);
		setHeadText(tmpText);
	}
	/*
	else if(tick_counter == mul * 400)
	{
		toStatusScreen();
	}
	else if(tick_counter == mul * 500)
	{
		toMainScreen();
		tick_counter = 0;
	}
	*/
	tick_counter++;
#endif
}

void Model::message_parser(struct message *message)
{
#ifndef SIMULATOR
	switch(message->cmd)
	{
		case WALLET_INIT:
			toInitScreen();
			break;

		case WALLET_STATUS:
			toStatusScreen();
			tmpText[0] = 0;
			break;

		case WALLET_ENTER_PIN:
			toPinScreen();
			pinScreenState = PIN_ENTER;
			touchgfx::Unicode::strncpy(tmpText, "Enter Your PIN-code", TEXT_SIZE);
			break;

		case WALLET_SET_PIN:
			toPinScreen();
			pinScreenState = PIN_SET;
			touchgfx::Unicode::strncpy(tmpText, "Set Your PIN-code", TEXT_SIZE);
			break;

		case WALLET_ENTER_MS:
			toPinScreen();
			pinScreenState = MS_ENTER;
			touchgfx::Unicode::strncpy(tmpText, "Enter Your Mnemonic Seed", TEXT_SIZE);
			break;

		case WALLET_SET_MS:
			toPinScreen();
			pinScreenState = MS_SET;
			touchgfx::Unicode::strncpy(tmpText, "Set Your Mnemonic Seed", TEXT_SIZE);
			break;

		case WALLET_TRANSACTION:
			toMainScreen();
			transaction = static_cast<struct transaction*>(message->data);
			break;

		case INIT_PINCODE:
			toPincodeScreen();
			static const char *init_pin = "Init PINCODE";
			touchgfx::Unicode::strncpy(tmpText, init_pin, TEXT_SIZE);
			break;

		case WALLET_WRONG_PINCODE:
			static const char *wrong_pin = "Wrong PINCODE";
			touchgfx::Unicode::strncpy(tmpText, wrong_pin, TEXT_SIZE);
			setDialogText(tmpText);
			break;

		case TRANSACTION_CONFIRMED:
			toStatusScreen();
			static const char *transaction_confirmed = "Transaction send";
			touchgfx::Unicode::strncpy(tmpText, transaction_confirmed, TEXT_SIZE);
			break;
	}
#endif
}

/************************************ Control logic ****************************************/
void Model::walletTransaction(struct transaction *trans)
{
	modelListener->walletTransaction(trans);
}

void Model::setDialogText(touchgfx::Unicode::UnicodeChar *text)
{
	modelListener->setDialogText(text);
}

void Model::setHeadText(touchgfx::Unicode::UnicodeChar *text)
{
	modelListener->setHeadText(text);
}

void Model::walletStatus(struct wallet_status *status)
{
	modelListener->walletStatus(status);
}

void Model::clearWallet()
{
	tx_mess.cmd = WALLET_CMD_CLEAR;
	xQueueSend(lcd_to_card, static_cast<void*>(&tx_mess), 0);
}

void Model::initWallet()
{
	tx_mess.cmd = WALLET_CMD_INIT;
	xQueueSend(lcd_to_card, static_cast<void*>(&tx_mess), 0);
}

void Model::restoreWallet()
{
	tx_mess.cmd = WALLET_CMD_RESTORE;
	xQueueSend(lcd_to_card, static_cast<void*>(&tx_mess), 0);
}

void Model::pincodeEntered(int pincode)
{
	pin = pincode;

#ifndef SIMULATOR
	struct message pin_data = {
			.cmd = WALLET_PINCODE,
			.data = static_cast<void*>(&pin)
	};

	xQueueSend(lcd_to_card, static_cast<void*>(&pin_data), 0);
#endif
}

void Model::msEntered(Unicode::UnicodeChar *mnemonic)
{
	int i = 0;
	int j = 0;

	memset(mnemonicSeed, 0, MNEMONIC_SIZE);

	while(i < MNEMONIC_SIZE && mnemonic[i] != 0)
	{
		if(mnemonic[i] != '\n')
		{
			mnemonicSeed[j++] = static_cast<char>(mnemonic[i]);
		}
		i++;
	}
	tx_mess.cmd = WALLET_MNEMONIC;
	tx_mess.data = static_cast<void*>(&mnemonicSeed);
	xQueueSend(lcd_to_card, static_cast<void*>(&tx_mess), 0);
}

/************************************ Screens switching ************************************/
void Model::toMainScreen()
{
	modelListener->toMainScreen();
}

void Model::toStatusScreen()
{
	modelListener->toStatusScreen();
}

void Model::toPincodeScreen()
{
	modelListener->toPincodeScreen();
}

void Model::toPinScreen()
{
	modelListener->toPinScreen();
}

void Model::toInitScreen()
{
	modelListener->toInitScreen();
}

void Model::showPinKeyboard()
{
	modelListener->showPinKeyboard();
}

void Model::showMSKeyboard()
{
	modelListener->showMSKeyboard();
}

void Model::showMSWindow()
{
	modelListener->showMSWindow();
}

/************************************ Screens entered callbacks *************************/
void Model::pincodeScreenEntered()
{
	setDialogText(tmpText);
}

void Model::transactionScreenEntered()
{
	static const char *message = "Confirmation";
	touchgfx::Unicode::strncpy(tmpText, message, TEXT_SIZE);
	setDialogText(tmpText);
#ifndef SIMULATOR

#endif
}

void Model::statusScreenEntered()
{
	tmpText[0] = 0;
	setDialogText(tmpText);
}

void Model::pinScreenEntered()
{
	switch(pinScreenState)
	{
		case PIN_ENTER:
		case PIN_SET:
			showPinKeyboard();
			break;

		case MS_ENTER:
			showMSKeyboard();
			break;

		case MS_SET:
			showMSWindow();
			break;

		default:
			break;
	}
	setHeadText(tmpText);
}

/************************************ Buttons callbacks ************************************/
void Model::cancelPressed()
{
#ifndef SIMULATOR
	tx_mess.cmd = WALLET_CANCEL_PRESSED;
	xQueueSend(lcd_to_card, static_cast<void*>(&tx_mess), 0);
#endif
}

void Model::confirmPressed()
{
#ifndef SIMULATOR
	toPinScreen();
	pinScreenState = PIN_ENTER;
	touchgfx::Unicode::strncpy(tmpText, "Enter Your PIN-code", TEXT_SIZE);
	tx_mess.cmd = WALLET_CONFIRM_PRESSED;
	xQueueSend(lcd_to_card, static_cast<void*>(&tx_mess), 0);
#endif
}
