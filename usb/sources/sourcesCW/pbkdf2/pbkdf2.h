/*
 * pbkdf2.h
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#ifndef SOURCES_SOURCESCW_PBKDF2_PBKDF2_H_
#define SOURCES_SOURCESCW_PBKDF2_PBKDF2_H_


#include <stdint.h>
#include "sha2.h"

typedef unsigned char BYTE;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

typedef struct _PBKDF2_HMAC_SHA256_CTX {
	UINT32 odig[SHA256_DIGEST_LENGTH / sizeof(UINT32)];
	UINT32 idig[SHA256_DIGEST_LENGTH / sizeof(UINT32)];
	UINT32 f[SHA256_DIGEST_LENGTH / sizeof(UINT32)];
	UINT32 g[SHA256_BLOCK_LENGTH / sizeof(UINT32)];
	char first;
} PBKDF2_HMAC_SHA256_CTX;

typedef struct _PBKDF2_HMAC_SHA512_CTX {
	UINT64 odig[SHA512_DIGEST_LENGTH / sizeof(UINT64)];
	UINT64 idig[SHA512_DIGEST_LENGTH / sizeof(UINT64)];
	UINT64 f[SHA512_DIGEST_LENGTH / sizeof(UINT64)];
	UINT64 g[SHA512_BLOCK_LENGTH / sizeof(UINT64)];
	char first;
} PBKDF2_HMAC_SHA512_CTX;

void pbkdf2_hmac_sha256_Init(PBKDF2_HMAC_SHA256_CTX *pctx, const BYTE *pass, int passlen, const BYTE *salt, int saltlen);
void pbkdf2_hmac_sha256_Update(PBKDF2_HMAC_SHA256_CTX *pctx, UINT32 iterations);
void pbkdf2_hmac_sha256_Final(PBKDF2_HMAC_SHA256_CTX *pctx, BYTE *key);
void pbkdf2_hmac_sha256(const BYTE *pass, int passlen, const BYTE *salt, int saltlen, UINT32 iterations, BYTE *key);

void pbkdf2_hmac_sha512_Init(PBKDF2_HMAC_SHA512_CTX *pctx, const BYTE *pass, int passlen, const BYTE *salt, int saltlen);
void pbkdf2_hmac_sha512_Update(PBKDF2_HMAC_SHA512_CTX *pctx, UINT32 iterations);
void pbkdf2_hmac_sha512_Final(PBKDF2_HMAC_SHA512_CTX *pctx, BYTE *key);
void pbkdf2_hmac_sha512(const BYTE *pass, int passlen, const BYTE *salt, int saltlen, UINT32 iterations, BYTE *key);

#endif /* SOURCES_SOURCESCW_PBKDF2_PBKDF2_H_ */
