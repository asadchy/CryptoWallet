/*
 * key.h
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#ifndef SOURCESCW_CRYPTO_KEY_H_
#define SOURCESCW_CRYPTO_KEY_H_

typedef unsigned char BYTE;
#include <stdint.h>


void generatePrivateKey(BYTE *seed, int *tocenNum, BYTE *privKey);


void genKeyC(uint32_t *pinDef, int idCurrency, BYTE * privKey, BYTE * pubKey, int compressed);

void genKeyE(uint32_t *pinDef, int idCurrency, BYTE * privKey, BYTE * pubKey);

#endif /* SOURCESCW_CRYPTO_KEY_H_ */
