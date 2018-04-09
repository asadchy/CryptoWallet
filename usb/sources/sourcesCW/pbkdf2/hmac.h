/*
 * hmac.h
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#ifndef SOURCES_SOURCESCW_PBKDF2_HMAC_H_
#define SOURCES_SOURCESCW_PBKDF2_HMAC_H_

#include <stdint.h>
#include "sha2.h"
typedef unsigned char BYTE;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

typedef struct _HMAC_SHA256_CTX {
	BYTE o_key_pad[SHA256_BLOCK_LENGTH];
	SHA256_CTX ctx;
} HMAC_SHA256_CTX;

typedef struct _HMAC_SHA512_CTX {
	BYTE o_key_pad[SHA512_BLOCK_LENGTH];
	SHA512_CTX ctx;
} HMAC_SHA512_CTX;

void hmac_sha256_Init(HMAC_SHA256_CTX *hctx, const BYTE *key, const UINT32 keylen);
void hmac_sha256_Update(HMAC_SHA256_CTX *hctx, const BYTE *msg, const UINT32 msglen);
void hmac_sha256_Final(HMAC_SHA256_CTX *hctx, BYTE *hmac);
void hmac_sha256(const BYTE *key, const UINT32 keylen, const BYTE *msg, const UINT32 msglen, BYTE *hmac);
void hmac_sha256_prepare(const BYTE *key, const UINT32 keylen, UINT32 *opad_digest, UINT32 *ipad_digest);

void hmac_sha512_Init(HMAC_SHA512_CTX *hctx, const BYTE *key, const UINT32 keylen);
void hmac_sha512_Update(HMAC_SHA512_CTX *hctx, const BYTE *msg, const UINT32 msglen);
void hmac_sha512_Final(HMAC_SHA512_CTX *hctx, BYTE *hmac);
void hmac_sha512(const BYTE *key, const UINT32 keylen, const BYTE *msg, const UINT32 msglen, BYTE *hmac);
void hmac_sha512_prepare(const BYTE *key, const UINT32 keylen, UINT64 *opad_digest, UINT64 *ipad_digest);



#endif /* SOURCES_SOURCESCW_PBKDF2_HMAC_H_ */
