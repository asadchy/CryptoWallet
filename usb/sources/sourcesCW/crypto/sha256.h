/*
 * sha256.h
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#ifndef SOURCESCW_CRYPTO_SHA256_H_
#define SOURCESCW_CRYPTO_SHA256_H_
typedef unsigned char BYTE;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;


/*
(IN) BYTE* data - data for hashing
(IN) UINT32 sizeData - length data for hashing
(OUT) BYTE* outSha256 - Hash SHA_256
(OUT) UINT32 &sizeHash - Size of Hash
*/
void SHA_256(BYTE* data, UINT32 sizeData, BYTE* outSha256);

void DoubleSHA_256(BYTE* data, UINT32 sizeData, BYTE* outSha256);

void HMAC_SHA256(BYTE out[], BYTE *data, UINT32 data_len, BYTE *key, UINT32 key_len);


#endif /* SOURCESCW_CRYPTO_SHA256_H_ */
