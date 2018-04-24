#include "answer.h"
#include "crypto/key.h"
#include "crypto/address.h"
#include "crypto/sign.h"
#include "string.h"
#include "balance.h"
#include "pbkdf2/mnemonic.h"

typedef unsigned char BYTE;
typedef unsigned int UINT32;

void initWalletCMD_(int walletInit, uint32_t *pinDef)
{
	uint8_t seed[32] = {0};
	struct message messRes;
	int initWallet = 0;
	int restoreWalletPin = 0;
	int restoreWalletMS = 0;
	while (pinDef[0] != walletInit)
	{
		BYTE pin[5] = {0};
		pin[4] = '\0';
		int lenMnem = 0;
		BYTE mnemonic[120] = {0};
		initWallet=0;
		restoreWalletPin = 0;
		restoreWalletMS = 0;
		struct message messInit;
		vTaskDelay(500 / portTICK_PERIOD_MS);
		messInit.cmd = WALLET_INIT;
		xQueueSend(card_to_lcd, (void*)&messInit, 0);
		while (!initWallet)
		{
			if(xQueueReceive(lcd_to_card, (void*)&messInit, 0))
			{
				int cmd = messInit.cmd;
				switch(cmd)
				{
				case WALLET_CMD_RESTORE:
					messRes.cmd = WALLET_ENTER_PIN;
					xQueueSend(card_to_lcd, (void*)&messRes, 0);
					while (!restoreWalletPin)
					{
						if(xQueueReceive(lcd_to_card, (void*)&messRes, 0))
						{
							cmd = messRes.cmd;
							switch(cmd)
							{
							case WALLET_PINCODE:
								pinDef[1] = *(uint32_t*)messRes.data;
								pin[0] = (pinDef[1] - pinDef[1]%1000)/1000 + 48;
								pin[1] = ((pinDef[1] - pinDef[1]%100)/100)%10 + 48;
								pin[2] = ((pinDef[1] - pinDef[1]%10)/10)%10 + 48;
								pin[3] = pinDef[1]%10 + 48;
								mnemonicGenerate(pin, mnemonic, &lenMnem, 128);
								restoreWalletPin = 1;
								messRes.cmd = WALLET_ENTER_MS;
								xQueueSend(card_to_lcd, (void*)&messRes, 0);
								while(!restoreWalletMS)
								{
									if(xQueueReceive(lcd_to_card, (void*)&messRes, 0))
									{
										int pinToSeed = 1;
										cmd = messRes.cmd;
										switch(cmd)
										{
										case WALLET_MNEMONIC:
											if(strncmp((char*)mnemonic, (char*)messRes.data, lenMnem))
											{
												pinToSeed = 0;
											}
											if(pinToSeed == 1)
											{
												pinDef[0] = walletInit;
												seedFromMnemonic(mnemonic, lenMnem, pin, 4, seed);
												for(int i = 0; i < 32; i++)
												{
													pinDef[i+2] = seed[i];
												}
												//write_flash(pinDef, 34, PIN_ADDR);
											}
											restoreWalletMS = 1;
											initWallet = 1;
											break;
										case WALLET_CANCEL_PRESSED:
											initWallet = 1;
											restoreWalletPin = 1;
											restoreWalletMS = 1;
											break;
										}
									}
								}
								break;
							case WALLET_CANCEL_PRESSED:
								initWallet = 1;
								restoreWalletPin = 1;
								break;
							}
						}
					}
					break;
				case WALLET_CMD_INIT:
					messInit.cmd = WALLET_SET_PIN;
					xQueueSend(card_to_lcd, (void*)&messInit, 0);
					while (!initWallet)
						{
						if(xQueueReceive(lcd_to_card, (void*)&messInit, 0))
						{
							int cmd = messInit.cmd;
							switch(cmd)
							{
							case WALLET_PINCODE:
								pinDef[1] = *(uint32_t*)messInit.data;
								pin[0] = (pinDef[1] - pinDef[1]%1000)/1000 + 48;
							pin[1] = ((pinDef[1] - pinDef[1]%100)/100)%10 + 48;
							pin[2] = ((pinDef[1] - pinDef[1]%10)/10)%10 + 48;
							pin[3] = pinDef[1]%10 + 48;
							mnemonicGenerate(pin, mnemonic, &lenMnem, 128);
							seedFromMnemonic(mnemonic, lenMnem, pin, 4, seed);
							pinDef[0] = walletInit;
							for(int i = 0; i < 32; i++)
							{
								pinDef[i+2] = seed[i];
							}
							//write_flash(pinDef, 34, PIN_ADDR);
							messInit.cmd = WALLET_SET_MS;
							mnemonic[lenMnem] = '\0';
							messInit.data = (void*)mnemonic;
							initWallet = 1;
							xQueueSend(card_to_lcd, (void*)&messInit, 0);
							int temp = 0;
							while(!temp)
							{
								if(xQueueReceive(lcd_to_card, (void*)&messInit, 0))
								{
									temp=1;
								}
							}
							break;
							case WALLET_CANCEL_PRESSED:
								initWallet = 1;
							break;
							}
						}
						}
					break;
				}
			}
		}
	}
}

void answerCom(uint8_t *dataIn, uint32_t* lenIn, uint8_t *dataOut, uint32_t* lenOut, int *pinInit, uint32_t *pinDef){
	BYTE privateKey[32] = { 0 };
	BYTE publicKey[65] = { 0 };
	BYTE address[42] = { 0 };
	int addressLen = 0;
	struct message mess;
	static struct wallet_status statusW;

	switch(dataIn[0])
	{
	case 0x42:{//balance
		for(int i = 0; i<96; i++)
		{
			balanceWallet[i] = dataIn[i+1];
		}
		for(int i = 0; i<16; i++)
		{
			statusW.curr[0].amount[i] = dataIn[i + 1];
			statusW.curr[0].amount_dollars[i] = dataIn[i + 17];
			statusW.curr[1].amount[i] = dataIn[i + 33];
			statusW.curr[1].amount_dollars[i] = dataIn[i + 49];
			statusW.curr[2].amount[i] = dataIn[i + 65];
			statusW.curr[2].amount_dollars[i] = dataIn[i + 81];
		}
		statusW.curr[0].amount[15] = '\0';
		statusW.curr[0].amount_dollars[15] = '\0';
		statusW.curr[1].amount[15] = '\0';
		statusW.curr[1].amount_dollars[15] = '\0';
		statusW.curr[2].amount[15] = '\0';
		statusW.curr[2].amount_dollars[15] = '\0';

		statusW.curr[0].curr_name = BTC;
		statusW.curr[1].curr_name = ETH;
		statusW.curr[2].curr_name = LTC;

		statusW.num = 3;
		mess.data = (void*)&statusW;
		mess.cmd = WALLET_STATUS;
		xQueueSend(card_to_lcd, (void*)&mess, 0);
		*lenOut = 7;
		dataOut[0] = 0x43;
		dataOut[1] = 0x4f;
		dataOut[2] = 0x4d;
		dataOut[3] = 0x42;
		dataOut[4] = 0x02;
		dataOut[5] = 0x4f;
		dataOut[6] = 0x4b;
		break;
	}
	case 0x46:{ //findPort
		*lenOut = 7;
		dataOut[0] = 0x43;
		dataOut[1] = 0x4f;
		dataOut[2] = 0x4d;
		dataOut[3] = 0x46;
		dataOut[4] = 0x02;
		dataOut[5] = 0x4f;
		dataOut[6] = 0x4b;
		break;
	}
	case 0x50:{ //checkPin
		*lenOut = 6;
		dataOut[0] = 0x43;
		dataOut[1] = 0x4f;
		dataOut[2] = 0x4d;
		dataOut[3] = 0x50;
		dataOut[4] = 0x01;
		if(*pinInit == -1){
			dataOut[5] = 0x04;
		}else{
			if(*pinInit == pinDef[1]){
				dataOut[5] = 0x00;
			}
			if(*pinInit != pinDef[1]){
				dataOut[5] = 0x03;
			}
		}
		break;
	}
	case 0x4e:{ //numberOfCurrencies
		*lenOut = 6;
		dataOut[0] = 0x43;
		dataOut[1] = 0x4f;
		dataOut[2] = 0x4d;
		dataOut[3] = 0x4e;
		dataOut[4] = 0x01;
		dataOut[5] = 0x03;
		break;
	}
	case 0x43:{ //CurrencyInfo
		dataOut[0] = 0x43;
		dataOut[1] = 0x4f;
		dataOut[2] = 0x4d;
		if(*pinInit != pinDef[1]){
			dataOut[3] = 0x00;
			*lenOut = 4;
			break;
		}

		if(dataIn[1] == 0x00){//bitcoin

			dataOut[3] = 0x42;
			dataOut[4] = 0x54;
			dataOut[5] = 0x43;

			int compressed = 1;
			int idCur = 0;
			genKeyC(pinDef, idCur, privateKey, publicKey, compressed);
			addressBtc(publicKey, compressed, address, &addressLen);
			for(int i = 0; i<addressLen; i++){
				dataOut[i+7] = address[i];
			}
			dataOut[6] = addressLen + 7;
			*lenOut = addressLen + 7;
		}
		if(dataIn[1] == 0x01){//ethereum
			*lenOut = 47;
			dataOut[3] = 0x45;
			dataOut[4] = 0x54;
			dataOut[5] = 0x48;
			dataOut[6] = 47;
			int idCur = 1;
			genKeyE(pinDef, idCur, privateKey, publicKey);
			addressEth(publicKey, address);
			for(int i = 0; i<40; i++){
				dataOut[i+7] = address[i];
			}
		}
		if(dataIn[1] == 0x02){//litecoin

			dataOut[3] = 0x4c;
			dataOut[4] = 0x54;
			dataOut[5] = 0x43;

			int compressed = 1;
			int idCur = 2;
			genKeyC(pinDef, idCur, privateKey, publicKey, compressed);
			addressLtc(publicKey, compressed, address, &addressLen);
			for(int i = 0; i<addressLen; i++){
				dataOut[i+7] = address[i];
			}
			*lenOut = addressLen + 7;
			dataOut[6] = addressLen + 7;
		}
		break;
	}
	case 0x53:{ //signature

		if(dataIn[1] == 0x00){//bitcoin
			int amount = dataIn[2];
			BYTE mess[512] = {0};
			for(int i = 0; i < 32*amount; i++){
				mess[i] = dataIn[3+i];
			}
			BYTE valueTr[16] = {0};
			for(int i = 0; i<15; i++)
			{
				valueTr[i] = dataIn[i + 3 + 32*amount];
			}
			BYTE addr[34] = {0};
			for(int i=0; i<34; i++){
				addr[i] = dataIn[19 + 32*amount + i];
			}
			sign (0, amount, mess, valueTr, addr, dataOut, lenOut, pinInit, pinDef);
		}
		if(dataIn[1] == 0x02){//litecoin
			int amount = dataIn[2];
			BYTE mess[512] = {0};
			for(int i = 0; i < 32*amount; i++){
				mess[i] = dataIn[3+i];
			}
			BYTE valueTr[16] = {0};
			for(int i = 0; i<15; i++)
			{
				valueTr[i] = dataIn[i + 3 + 32*amount];
			}
			BYTE addr[34] = {0};
			for(int i=0; i<34; i++){
				addr[i] = dataIn[19 + 32*amount + i];
			}
			sign (2, amount, mess, valueTr, addr, dataOut, lenOut, pinInit, pinDef);
		}
		if(dataIn[1] == 0x01){//ethereum
			int amount = dataIn[2];
			BYTE mess[512] = {0};
			for(int i = 0; i < 32*amount; i++){
				mess[i] = dataIn[3+i];
			}
			BYTE valueTr[16] = {0};
			for(int i = 0; i<15; i++)
			{
				valueTr[i] = dataIn[i + 3 + 32*amount];
			}
			BYTE addr[42] = {0};
			for(int i=0; i<42; i++){
				addr[i] = dataIn[19 + 32*amount + i];
			}
			sign (1, amount, mess, valueTr, addr, dataOut, lenOut, pinInit, pinDef);
		}
	break;
	}
	default:{
		dataIn[0] = 0x00;;
		dataOut[0] = 0x00;
		*lenOut = 0;
		*lenIn = 0;
		break;
	}
	}
}

int dataToBuffer(uint8_t *dataIn, uint32_t *lenIn, uint8_t *buffer, uint32_t *lenBuf, uint32_t *send, int *pinInit, uint32_t *pinDef){

	for(int i = (*lenBuf); i<((*lenBuf)+(*lenIn)); i++){
		buffer[i] = dataIn[i-(*lenBuf)];
	}
	(*lenBuf) = (*lenBuf)+(*lenIn);
	uint8_t tempBuf[1800] = {0};
	uint32_t start = 0;
	uint32_t end = 0;
	if((*lenBuf) > 5){
		for(int i=0; i<((*lenBuf)-1);i++){
			if(buffer[i] == 0x9c && buffer[i+1] == 0x9c){
				for(int j = i+2; j<((*lenBuf)-1);j++){
					if(buffer[j] == 0x9a && buffer[j+1] == 0x9a){
						start = i+2;
						end = j;
						for(int i = start; i<end; i++){
							tempBuf[i-start] = buffer[i];
						}
						*send=1;
						uint32_t lenTempBuf = end-start;

						answerCom(tempBuf, &lenTempBuf,buffer, lenBuf, pinInit, pinDef);
						return 0;
					}
				}
			}
		}
	}
	return 0;
}


void sign (int id, int amount, BYTE *mess, BYTE *valueTr, BYTE *addr, BYTE *out, uint32_t *lenOut, int *pinInit, uint32_t *pinDef){
	int numCheckPin = 0;
	BYTE privateKey[32] = { 0 };
	BYTE publicKey[65] = { 0 };
	BYTE signature[130] = {0};
	static struct message message;
	static struct transaction transaction;
	int blocked = 0;
	static struct wallet_status statusW;
	BYTE mesToSign[32] = {0};
	out[0] = 0x43;
	out[1] = 0x4f;
	out[2] = 0x4d;
	out[3] = amount;

	if(id==0) {transaction.curr_name = BTC;}
	if(id==1) {transaction.curr_name = ETH;}
	if(id==2) {transaction.curr_name = LTC;}
	for(int i = 0; i<16; i++)
	{
		transaction.value[i] = valueTr[i];
	}
	if( id == 0 || id ==2 ){
		for(int i = 0; i < 34; i++){
			transaction.addr[i] = addr[i];
		}
		transaction.addr[34] = '\0';
	}
	if( id == 1 ){
		for(int i = 0; i < 42; i++){
			transaction.addr[i] = addr[i];
		}
		transaction.addr[42] = '\0';
	}
	message.cmd = WALLET_TRANSACTION;
	message.data = (void*)&transaction;
	xQueueSend(card_to_lcd, (void*)&message, 0);
	int ans = 0;
	while(!ans){
		if(xQueueReceive(lcd_to_card, (void*)&message, portMAX_DELAY))
		{
	int cmd = message.cmd;
	switch(cmd)
	{
	case WALLET_PINCODE:
		do{
			int tempLen = 4;
			if(pinDef[1] == (*(int*)message.data)){
				*pinInit = *(int*)message.data;
				numCheckPin = 0;
				int compressed = 1;
				for(int j = 0; j < amount; j++){
					for(int i=0; i<32; i++){
						mesToSign[i] = mess[i + j*32];
					}
					taskENTER_CRITICAL();
					if(id == 0 || id == 2) {
						genKeyC(pinDef, id, privateKey, publicKey, compressed);
						int lenOutData = 0;
						getSignBtc(privateKey, compressed, mesToSign, signature, &lenOutData);
						out[tempLen] = lenOutData;
						tempLen += 1;
						for(int i=0; i<lenOutData; i++){
							out[i+tempLen] = signature[i];
						}
						tempLen += lenOutData;
					}
					if(id == 1){
						genKeyE(pinDef, id, privateKey, publicKey);
						getSignEther(privateKey, mesToSign, signature);
						tempLen = 5+65;
						out[4] = 65;
						for(int i=0; i<65; i++){
							out[i+5] = signature[i];
						}
					}
					taskEXIT_CRITICAL();
				}
				message.cmd = TRANSACTION_CONFIRMED;
				xQueueSend(card_to_lcd, (void*)&message, 0);
				*lenOut = tempLen;
			}else{
				numCheckPin ++;
				if(numCheckPin>2){
					message.cmd = WALLET_BLOCKED;
					xQueueSend(card_to_lcd, (void*)&message, 0);
					vTaskDelay( 5000 );
					numCheckPin = 0;
					out[3] = 0x01;
					out[4] = 0x04;
					*lenOut = 5;
					blocked = 1;
				}else{
				out[3] = 0x01;
				out[4] = 0x04;
				*lenOut = 5;
				int ostNum = 3-numCheckPin;
				message.data = (void*)&ostNum;
				message.cmd = WALLET_WRONG_PINCODE;
				xQueueSend(card_to_lcd, (void*)&message, 0);
				xQueueReceive(lcd_to_card, (void*)&message, portMAX_DELAY);}
			}
		}while(((pinDef[1] != (*(int*)message.data))||(numCheckPin!=0)) && (blocked == 0));
		ans=1;
		for(int i = 0; i<16; i++)
		{
			statusW.curr[0].amount[i] = balanceWallet[i];
			statusW.curr[0].amount_dollars[i] = balanceWallet[i + 16];
			statusW.curr[1].amount[i] = balanceWallet[i + 32];
			statusW.curr[1].amount_dollars[i] = balanceWallet[i + 48];
			statusW.curr[2].amount[i] = balanceWallet[i + 64];
			statusW.curr[2].amount_dollars[i] = balanceWallet[i + 80];
		}
		statusW.curr[0].amount[15] = '\0';
		statusW.curr[0].amount_dollars[15] = '\0';
		statusW.curr[1].amount[15] = '\0';
		statusW.curr[1].amount_dollars[15] = '\0';
		statusW.curr[2].amount[15] = '\0';
		statusW.curr[2].amount_dollars[15] = '\0';

		statusW.curr[0].curr_name = BTC;
		statusW.curr[1].curr_name = ETH;
		statusW.curr[2].curr_name = LTC;

		statusW.num = 3;
		message.data = (void*)&statusW;
		message.cmd = WALLET_STATUS;
		xQueueSend(card_to_lcd, (void*)&message, 0);
		break;

	case WALLET_CANCEL_PRESSED:
		out[3] = 0x00;
		*lenOut = 4;
		ans=1;
		for(int i = 0; i<16; i++)
		{
			statusW.curr[0].amount[i] = balanceWallet[i];
			statusW.curr[0].amount_dollars[i] = balanceWallet[i + 16];
			statusW.curr[1].amount[i] = balanceWallet[i + 32];
			statusW.curr[1].amount_dollars[i] = balanceWallet[i + 48];
			statusW.curr[2].amount[i] = balanceWallet[i + 64];
			statusW.curr[2].amount_dollars[i] = balanceWallet[i + 80];
		}
		statusW.curr[0].amount[15] = '\0';
		statusW.curr[0].amount_dollars[15] = '\0';
		statusW.curr[1].amount[15] = '\0';
		statusW.curr[1].amount_dollars[15] = '\0';
		statusW.curr[2].amount[15] = '\0';
		statusW.curr[2].amount_dollars[15] = '\0';

		statusW.curr[0].curr_name = BTC;
		statusW.curr[1].curr_name = ETH;
		statusW.curr[2].curr_name = LTC;

		statusW.num = 3;
		message.data = (void*)&statusW;
		message.cmd = WALLET_STATUS;
		xQueueSend(card_to_lcd, (void*)&message, 0);
		break;

	}
	}
	}
}
