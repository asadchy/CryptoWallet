/*
 * transformSign.c
 *
 *  Created on: Mar 9, 2018
 *      Author: User
 */
#include "transformSign.h"

void transfomS(BYTE *sign, int *len, BYTE *out){
	BYTE n[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
				 0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B,
				 0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x41 };
			BYTE lo_s[32] = { 0 };
			if (!(sign[32] < 0x80)) {
				for (int i = 31; i > 0; i--)
				{
					if ((int)n[i]<(int)sign[i+32]) {
						lo_s[i] = 256 + (int)n[i] - (int)sign[i+32];
						n[i - 1]--;
					}
					else {
						lo_s[i] = (int)n[i] - (int)sign[i+32];
					}
				}
				lo_s[0] = (int)n[0] - (int)sign[32];
				for (int i = 0; i < 32; i++)
				{
					out[i] = lo_s[i];
				}
			}else{
				for (int i = 0; i < 32; i++)
				{
					out[i] = sign[i+32];
				}
			}
	*len = 32;
}


void transfomR(BYTE *sign, int *len, BYTE *out){
	if(!(sign[0] < 0x80)){
		for(int i = 32; i>0; i--){
			out[i] = sign[i-1];
		}
		out[0] = 0x00;
		*len = 33;
	} else{
		for(int i = 31; i>-1; i--){
			out[i] = sign[i];
		}
		*len = 32;
	}
}


void transfomSig(BYTE * sign, BYTE *pub, int *len){
	int lenS = 32;
	int lenR = 0;
	BYTE outS[32] = {0};
	BYTE outR[33] = {0};
	transfomR(sign, &lenR, outR);
	transfomS(sign, &lenS, outS);

	int lenSig = 6 + lenS + lenR;
	BYTE sig[100] = { 0 };
	sig[0] = 0x30;
	sig[1] = 4 + lenS + lenR;
	sig[2] = 0x02;
	sig[3] = lenR;
	for(int i = 0; i < lenR; i++){
		sig[i+4] = outR[i];
	}
	sig[4 + lenR] = 0x02;
	sig[5 + lenR] = lenS;
	for(int i = 0; i < lenS; i++){
			sig[i+6+lenR] = outS[i];
		}
	//////////////////////////////
	int lenPubKey = 33;
	int lenOutData = (4 + lenSig + lenPubKey) * 2;
	sign[0] = 3 + lenSig + lenPubKey;
	sign[1] = 1 + lenSig;
	memcpy(sign + 2, sig, lenSig);
	sign[2 + lenSig] = 0x01;
	sign[3 + lenSig] = lenPubKey;
	memcpy(sign + 4 + lenSig, pub, lenPubKey);
	*len = lenOutData;
}

