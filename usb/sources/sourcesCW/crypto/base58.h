/*
 * base58.h
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#ifndef SOURCESCW_CRYPTO_BASE58_H_
#define SOURCESCW_CRYPTO_BASE58_H_
#include <stdint.h>
typedef unsigned char BYTE;

/*
(IN) BYTE *inData - data for encoding BASE58
(IN) int dataLen - length data for encoding BASE58
(OUT) BYTE *outData - data encoded BASE58
(OUT) int *outLen - length encoded data (int outBase58DataLength = 1000;)*/
void base58(BYTE *inData, int dataLen, BYTE *outData, int *outLen);



#endif /* SOURCESCW_CRYPTO_BASE58_H_ */
