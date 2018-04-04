/*
 * transformSign.h
 *
 *  Created on: Mar 9, 2018
 *      Author: User
 */

#ifndef SOURCESCW_CRYPTO_TRANSFORMSIGN_H_
#define SOURCESCW_CRYPTO_TRANSFORMSIGN_H_

typedef unsigned char BYTE;

void transfomS(BYTE *sign, int *len, BYTE *out, int *trans);
void transfomR(BYTE *sign, int *len, BYTE *out);
void transfomSig(BYTE * sign, BYTE *pub, int *len);

#endif /* SOURCESCW_CRYPTO_TRANSFORMSIGN_H_ */
