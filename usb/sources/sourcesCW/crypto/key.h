/*
 * key.h
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#ifndef SOURCESCW_CRYPTO_KEY_H_
#define SOURCESCW_CRYPTO_KEY_H_

typedef unsigned char BYTE;


void generatePrivateKey(BYTE* msg, int *tocenNum, BYTE *privKey);


void genKeyC(int pin, int idCurrency, BYTE * privKey, BYTE * pubKey, int compressed);

void genKeyE(int pin, int idCurrency, BYTE * privKey, BYTE * pubKey);

#endif /* SOURCESCW_CRYPTO_KEY_H_ */
