/*
 * ripemd160.h
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#ifndef SOURCESCW_CRYPTO_RIPEMD160_H_
#define SOURCESCW_CRYPTO_RIPEMD160_H_

typedef unsigned char BYTE;
typedef unsigned int UINT32;

/*
(IN) BYTE* inData - data for hashing
(IN) UINT32 inDataLength - length data for hashing
(OUT) BYTE* outDataHesh - Hesh RIPEMD160
(OUT) UINT32 &outDataHeshLenght - Size of Hash
*/
void ripemd160(const BYTE *inData, UINT32 inDataLength, BYTE* outDataHesh);

#endif /* SOURCESCW_CRYPTO_RIPEMD160_H_ */
