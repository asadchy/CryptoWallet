/*
 * sign.h
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#ifndef SOURCESCW_CRYPTO_SIGN_H_
#define SOURCESCW_CRYPTO_SIGN_H_

typedef unsigned char BYTE;

void getSignBtc(BYTE *privKey, int compressed, BYTE* hash, BYTE* outData, int *lenOutData);

void getSignEther(BYTE *privKey, BYTE* hash, BYTE* outData);

#endif /* SOURCESCW_CRYPTO_SIGN_H_ */
