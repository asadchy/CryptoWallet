/*
 * keccak.h
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#ifndef SOURCESCW_CRYPTO_KECCAK_H_
#define SOURCESCW_CRYPTO_KECCAK_H_

typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef unsigned char BYTE;

void keccak(const BYTE *inData, const UINT32 lenInData, BYTE * hashOut);

#endif /* SOURCESCW_CRYPTO_KECCAK_H_ */
