/*
 * hmac.c
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#include <string.h>

#include "hmac.h"
#include "memzero.h"

void hmac_sha256_Init(HMAC_SHA256_CTX *hctx, const BYTE *key, const UINT32 keylen)
{
	static BYTE i_key_pad[SHA256_BLOCK_LENGTH];
	memset(i_key_pad, 0, SHA256_BLOCK_LENGTH);
	if (keylen > SHA256_BLOCK_LENGTH) {
		sha256_Raw(key, keylen, i_key_pad);
	}
	else {
		memcpy(i_key_pad, key, keylen);
	}
	for (int i = 0; i < SHA256_BLOCK_LENGTH; i++) {
		hctx->o_key_pad[i] = i_key_pad[i] ^ 0x5c;
		i_key_pad[i] ^= 0x36;
	}
	sha256_Init(&(hctx->ctx));
	sha256_Update(&(hctx->ctx), i_key_pad, SHA256_BLOCK_LENGTH);
	memzero(i_key_pad, sizeof(i_key_pad));
}

void hmac_sha256_Update(HMAC_SHA256_CTX *hctx, const BYTE *msg, const UINT32 msglen)
{
	sha256_Update(&(hctx->ctx), msg, msglen);
}

void hmac_sha256_Final(HMAC_SHA256_CTX *hctx, BYTE *hmac)
{
	sha256_Final(&(hctx->ctx), hmac);
	sha256_Init(&(hctx->ctx));
	sha256_Update(&(hctx->ctx), hctx->o_key_pad, SHA256_BLOCK_LENGTH);
	sha256_Update(&(hctx->ctx), hmac, SHA256_DIGEST_LENGTH);
	sha256_Final(&(hctx->ctx), hmac);
	memzero(hctx, sizeof(HMAC_SHA256_CTX));
}

void hmac_sha256(const BYTE *key, const UINT32 keylen, const BYTE *msg, const UINT32 msglen, BYTE *hmac)
{
	static HMAC_SHA256_CTX hctx;
	hmac_sha256_Init(&hctx, key, keylen);
	hmac_sha256_Update(&hctx, msg, msglen);
	hmac_sha256_Final(&hctx, hmac);
}

void hmac_sha256_prepare(const BYTE *key, const UINT32 keylen, UINT32 *opad_digest, UINT32 *ipad_digest)
{
	static UINT32 key_pad[SHA256_BLOCK_LENGTH / sizeof(UINT32)];

	memzero(key_pad, sizeof(key_pad));
	if (keylen > SHA256_BLOCK_LENGTH) {
		static SHA256_CTX context;
		sha256_Init(&context);
		sha256_Update(&context, key, keylen);
		sha256_Final(&context, (BYTE*)key_pad);
	}
	else {
		memcpy(key_pad, key, keylen);
	}

	/* compute o_key_pad and its digest */
	for (int i = 0; i < SHA256_BLOCK_LENGTH / (int)sizeof(UINT32); i++) {
		UINT32 data;
#if BYTE_ORDER == LITTLE_ENDIAN
		REVERSE32(key_pad[i], data);
#else
		data = key_pad[i];
#endif
		key_pad[i] = data ^ 0x5c5c5c5c;
	}
	sha256_Transform(sha256_initial_hash_value, key_pad, opad_digest);

	/* convert o_key_pad to i_key_pad and compute its digest */
	for (int i = 0; i < SHA256_BLOCK_LENGTH / (int)sizeof(uint32_t); i++) {
		key_pad[i] = key_pad[i] ^ 0x5c5c5c5c ^ 0x36363636;
	}
	sha256_Transform(sha256_initial_hash_value, key_pad, ipad_digest);
	memzero(key_pad, sizeof(key_pad));
}

void hmac_sha512_Init(HMAC_SHA512_CTX *hctx, const BYTE *key, const UINT32 keylen)
{
	static BYTE i_key_pad[SHA512_BLOCK_LENGTH];
	memset(i_key_pad, 0, SHA512_BLOCK_LENGTH);
	if (keylen > SHA512_BLOCK_LENGTH) {
		sha512_Raw(key, keylen, i_key_pad);
	}
	else {
		memcpy(i_key_pad, key, keylen);
	}
	for (int i = 0; i < SHA512_BLOCK_LENGTH; i++) {
		hctx->o_key_pad[i] = i_key_pad[i] ^ 0x5c;
		i_key_pad[i] ^= 0x36;
	}
	sha512_Init(&(hctx->ctx));
	sha512_Update(&(hctx->ctx), i_key_pad, SHA512_BLOCK_LENGTH);
	memzero(i_key_pad, sizeof(i_key_pad));
}

void hmac_sha512_Update(HMAC_SHA512_CTX *hctx, const BYTE *msg, const UINT32 msglen)
{
	sha512_Update(&(hctx->ctx), msg, msglen);
}

void hmac_sha512_Final(HMAC_SHA512_CTX *hctx, BYTE *hmac)
{
	sha512_Final(&(hctx->ctx), hmac);
	sha512_Init(&(hctx->ctx));
	sha512_Update(&(hctx->ctx), hctx->o_key_pad, SHA512_BLOCK_LENGTH);
	sha512_Update(&(hctx->ctx), hmac, SHA512_DIGEST_LENGTH);
	sha512_Final(&(hctx->ctx), hmac);
	memzero(hctx, sizeof(HMAC_SHA512_CTX));
}

void hmac_sha512(const BYTE *key, const UINT32 keylen, const BYTE *msg, const UINT32 msglen, BYTE *hmac)
{
	HMAC_SHA512_CTX hctx;
	hmac_sha512_Init(&hctx, key, keylen);
	hmac_sha512_Update(&hctx, msg, msglen);
	hmac_sha512_Final(&hctx, hmac);
}

void hmac_sha512_prepare(const BYTE *key, const UINT32 keylen, UINT64 *opad_digest, UINT64 *ipad_digest)
{
	static UINT64 key_pad[SHA512_BLOCK_LENGTH / sizeof(UINT64)];

	memzero(key_pad, sizeof(key_pad));
	if (keylen > SHA512_BLOCK_LENGTH) {
		static  SHA512_CTX context;
		sha512_Init(&context);
		sha512_Update(&context, key, keylen);
		sha512_Final(&context, (BYTE*)key_pad);
	}
	else {
		memcpy(key_pad, key, keylen);
	}

	/* compute o_key_pad and its digest */
	for (int i = 0; i < SHA512_BLOCK_LENGTH / (int)sizeof(UINT64); i++) {
		UINT64 data;
#if BYTE_ORDER == LITTLE_ENDIAN
		REVERSE64(key_pad[i], data);
#else
		data = key_pad[i];
#endif
		key_pad[i] = data ^ 0x5c5c5c5c5c5c5c5c;
	}
	sha512_Transform(sha512_initial_hash_value, key_pad, opad_digest);

	/* convert o_key_pad to i_key_pad and compute its digest */
	for (int i = 0; i < SHA512_BLOCK_LENGTH / (int)sizeof(UINT64); i++) {
		key_pad[i] = key_pad[i] ^ 0x5c5c5c5c5c5c5c5c ^ 0x3636363636363636;
	}
	sha512_Transform(sha512_initial_hash_value, key_pad, ipad_digest);
	memzero(key_pad, sizeof(key_pad));
}


