#include "answer.h"
#include "crypto/key.h"
#include "crypto/address.h"
#include "crypto/sign.h"
#include "string.h"
#include "data.hpp"
//#include "flash.h"
typedef unsigned char BYTE;
typedef unsigned int UINT32;

void answerCom(uint8_t *dataIn, uint32_t* lenIn, uint8_t *dataOut, uint32_t* lenOut, int *pinInit){
	int numCheckPin = 0;
	BYTE privateKey[32] = { 0 };
	BYTE publicKey[65] = { 0 };
	BYTE address[40] = { 0 };
	int addressLen = 0;
	uint32_t pinDef[2] = {0};
	//read_flash2(pinDef, 2);
	pinDef[1] = 1234;
	BYTE signature[130] = {0};
	static struct message message;
	static struct transaction transaction;


	switch(dataIn[0]){
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
			genKeyC(*pinInit, idCur, privateKey, publicKey, compressed);
			addressBtc(publicKey, compressed, address, &addressLen);
			for(int i = 0; i<addressLen; i++){
				dataOut[i+7] = address[i];
			}
			dataOut[6] = addressLen + 7;
			*lenOut = addressLen + 7;
		}
		if(dataIn[1] == 0x01){//ethereum
			*lenOut = 27;
			dataOut[3] = 0x45;
			dataOut[4] = 0x54;
			dataOut[5] = 0x48;
			dataOut[6] = 27;
			int idCur = 1;
			genKeyE(*pinInit, idCur, privateKey, publicKey);
			addressEth(publicKey, address);
			for(int i = 0; i<20; i++){
				dataOut[i+7] = address[i];
			}
		}
		if(dataIn[1] == 0x02){//litecoin

			dataOut[3] = 0x4c;
			dataOut[4] = 0x54;
			dataOut[5] = 0x43;

			int compressed = 1;
			int idCur = 2;
			genKeyC(*pinInit, idCur, privateKey, publicKey, compressed);
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
		dataOut[0] = 0x43;
		dataOut[1] = 0x4f;
		dataOut[2] = 0x4d;
		if(dataIn[1] == 0x00){//bitcoin
			BYTE mess[32] = {0};
			for(int i=0; i<32; i++){
				mess[i] = dataIn[i+2];
			}
			int valueTr = 0;
			valueTr = ((int)dataIn[34]*16777216 + (int)dataIn[35]*65536 + (int)dataIn[36]*256 + (int)dataIn[37]);

			memcpy(transaction.curr_name, "Bitcoin", 8);
			transaction.value = valueTr;
			for(int i = 0; i < 34; i++){
				transaction.addr[i] = dataIn[i+38];
			}

			message.cmd = TRANSACTION;
			message.data = (void*)&transaction;
			xQueueSend(card_to_lcd, (void*)&message, 0);

			xQueueReceive(lcd_to_card, (void*)&message, portMAX_DELAY);
			int cmd = message.cmd;
			switch(cmd)
			{
				case PINCODE:
do{
					if(pinDef[1] == (*(int*)message.data)){
						*pinInit = *(int*)message.data;
						numCheckPin = 0;
						int compressed = 1;
						int idCur = 0;
						taskENTER_CRITICAL();
						genKeyC(*pinInit, idCur, privateKey, publicKey, compressed);
						int lenOutData = 0;
						getSignBtc(privateKey, compressed, mess, signature, &lenOutData);
						*lenOut = 4+lenOutData;
						dataOut[3] = lenOutData;
						for(int i=0; i<lenOutData; i++){
							dataOut[i+4] = signature[i];
						}
						taskEXIT_CRITICAL();
						message.cmd = TRANSACTION_CONFIRMED;
						xQueueSend(card_to_lcd, (void*)&message, 0);
					}else{
						numCheckPin ++;
						if(numCheckPin>2){
							message.cmd = BLOCKED;
							xQueueSend(card_to_lcd, (void*)&message, 0);
						}
						dataOut[3] = 0x01;
						dataOut[4] = 0x04;
						*lenOut = 5;
						message.cmd = WRONG_PINCODE;
						xQueueSend(card_to_lcd, (void*)&message, 0);
						xQueueReceive(lcd_to_card, (void*)&message, portMAX_DELAY);
					}
}while((pinDef[1] != (*(int*)message.data))||(numCheckPin!=0));
					break;

				case TRANSACTION_CANCELED:
					dataOut[3] = 0x00;
					*lenOut = 4;
					break;
			}
		}
		if(dataIn[1] == 0x02){//litecoin
			BYTE mess[32] = {0};
			for(int i=0; i<32; i++){
				mess[i] = dataIn[i+2];
			}
			int valueTr = 0;
			valueTr = ((int)dataIn[34]*16777216 + (int)dataIn[35]*65536 + (int)dataIn[36]*256 + (int)dataIn[37]);

			memcpy(transaction.curr_name, "Litecoin", 9);
			transaction.value = valueTr;
			for(int i = 0; i < 34; i++){
				transaction.addr[i] = dataIn[i+38];
			}

			message.cmd = TRANSACTION;
			message.data = (void*)&transaction;
			xQueueSend(card_to_lcd, (void*)&message, 0);

			xQueueReceive(lcd_to_card, (void*)&message, portMAX_DELAY);
			int cmd = message.cmd;
			switch(cmd)
			{
				case PINCODE:
					do{
						if(pinDef[1] == (*(int*)message.data)){
							*pinInit = *(int*)message.data;
							numCheckPin = 0;
							int compressed = 1;
							int idCur = 2;
							taskENTER_CRITICAL();
							genKeyC(*pinInit, idCur, privateKey, publicKey, compressed);
							int lenOutData = 0;
							getSignBtc(privateKey, compressed, mess, signature, &lenOutData);
							*lenOut = 4+lenOutData;
							dataOut[3] = lenOutData;
							for(int i=0; i<lenOutData; i++){
								dataOut[i+4] = signature[i];
							}
							taskEXIT_CRITICAL();
							message.cmd = TRANSACTION_CONFIRMED;
							xQueueSend(card_to_lcd, (void*)&message, 0);
						}else{
							numCheckPin ++;
							if(numCheckPin>2){
								message.cmd = BLOCKED;
								xQueueSend(card_to_lcd, (void*)&message, 0);
							}
							dataOut[3] = 0x01;
							dataOut[4] = 0x04;
							*lenOut = 5;
							message.cmd = WRONG_PINCODE;
							xQueueSend(card_to_lcd, (void*)&message, 0);
							xQueueReceive(lcd_to_card, (void*)&message, portMAX_DELAY);
						}
					}while((pinDef[1] != (*(int*)message.data))||(numCheckPin!=0));

					break;

				case TRANSACTION_CANCELED:
					dataOut[3] = 0x00;
					*lenOut = 4;
					break;
			}
		}
		if(dataIn[1] == 0x01){//ethereum
			BYTE mess[32] = {0};
			for(int i=0; i<32; i++){
				mess[i] = dataIn[i+2];
			}
			int valueTr = 0;
			valueTr = ((int)dataIn[34]*16777216 + (int)dataIn[35]*65536 + (int)dataIn[36]*256 + (int)dataIn[37]);

			memcpy(transaction.curr_name, "Ethereum", 9);
			transaction.value = valueTr;
			for(int i = 0; i < 42; i++){
				transaction.addr[i] = dataIn[i+38];
			}

			message.cmd = TRANSACTION;
			message.data = (void*)&transaction;
			xQueueSend(card_to_lcd, (void*)&message, 0);

			xQueueReceive(lcd_to_card, (void*)&message, portMAX_DELAY);
			int cmd = message.cmd;
			switch(cmd)
			{
				case PINCODE:
					do{
						if(pinDef[1] == (*(int*)message.data)){
							*pinInit = *(int*)message.data;
							numCheckPin = 0;
							int idCur = 1;
							taskENTER_CRITICAL();
							genKeyE(*pinInit, idCur, privateKey, publicKey);
							getSignEther(privateKey, mess, signature);
							*lenOut = 4+65;
							dataOut[3] = 65;
							for(int i=0; i<65; i++){
								dataOut[i+4] = signature[i];
							}
							taskEXIT_CRITICAL();
							message.cmd = TRANSACTION_CONFIRMED;
							xQueueSend(card_to_lcd, (void*)&message, 0);
						}else{
							numCheckPin ++;
							if(numCheckPin>2){
								message.cmd = BLOCKED;
								xQueueSend(card_to_lcd, (void*)&message, 0);
							}
							dataOut[3] = 0x01;
							dataOut[4] = 0x04;
							*lenOut = 5;
							message.cmd = WRONG_PINCODE;
							xQueueSend(card_to_lcd, (void*)&message, 0);
							xQueueReceive(lcd_to_card, (void*)&message, portMAX_DELAY);
						}
					}while((pinDef[1] != (*(int*)message.data))||(numCheckPin!=0));

					break;

				case TRANSACTION_CANCELED:
					dataOut[3] = 0x00;
					*lenOut = 4;
					break;
			}
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

int dataToBuffer(uint8_t *dataIn, uint32_t *lenIn, uint8_t *buffer, uint32_t *lenBuf, uint32_t *send, int *pinInit){

	for(int i = (*lenBuf); i<((*lenBuf)+(*lenIn)); i++){
		buffer[i] = dataIn[i-(*lenBuf)];
	}
	(*lenBuf) = (*lenBuf)+(*lenIn);
	uint8_t tempBuf[100] = {0};
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
						answerCom(tempBuf, &lenTempBuf,buffer, lenBuf, pinInit);
						return 0;
					}
				}
			}
		}
	}
	return 0;
}

void checkPin(int *pinInit){
	uint32_t pinDef[2] = {0};
	//read_flash2(pinDef, 8);
	int numCheckPin = 0;
	static struct message message;
	while((*pinInit)!= pinDef[1] || numCheckPin < 3){
		message.cmd = WRONG_PINCODE;
		xQueueSend(card_to_lcd, (void*)&message, 0);
		xQueueReceive(lcd_to_card, (void*)&message, portMAX_DELAY);
		if(pinDef[1] == (*(int*)message.data)){
			*pinInit = *(int*)message.data;
			numCheckPin = 0;
			message.cmd = TO_STATUS;
			xQueueSend(card_to_lcd, (void*)&message, 0);
		}else{
			numCheckPin++;
		}
		if(numCheckPin >2){
			message.cmd = BLOCKED;
			xQueueSend(card_to_lcd, (void*)&message, 0);
		}
	}
}
