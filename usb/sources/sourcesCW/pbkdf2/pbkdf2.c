/*
 * pbkdf2.c
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#include <string.h>

#include "pbkdf2.h"
#include "hmac.h"
#include "memzero.h"

void pbkdf2_hmac_sha256_Init(PBKDF2_HMAC_SHA256_CTX *pctx, const BYTE *pass, int passlen, const BYTE *salt, int saltlen)
{
	SHA256_CTX ctx;
	UINT32 blocknr = 1;
#if BYTE_ORDER == LITTLE_ENDIAN
	REVERSE32(blocknr, blocknr);
#endif

	hmac_sha256_prepare(pass, passlen, pctx->odig, pctx->idig);
	memset(pctx->g, 0, sizeof(pctx->g));
	pctx->g[8] = 0x80000000;
	pctx->g[15] = (SHA256_BLOCK_LENGTH + SHA256_DIGEST_LENGTH) * 8;

	memcpy(ctx.state, pctx->idig, sizeof(pctx->idig));
	ctx.bitcount = SHA256_BLOCK_LENGTH * 8;
	sha256_Update(&ctx, salt, saltlen);
	sha256_Update(&ctx, (BYTE*)&blocknr, sizeof(blocknr));
	sha256_Final(&ctx, (BYTE*)pctx->g);
#if BYTE_ORDER == LITTLE_ENDIAN
	for (UINT32 k = 0; k < SHA256_DIGEST_LENGTH / sizeof(UINT32); k++) {
		REVERSE32(pctx->g[k], pctx->g[k]);
	}
#endif
	sha256_Transform(pctx->odig, pctx->g, pctx->g);
	memcpy(pctx->f, pctx->g, SHA256_DIGEST_LENGTH);
	pctx->first = 1;
}

void pbkdf2_hmac_sha256_Update(PBKDF2_HMAC_SHA256_CTX *pctx, UINT32 iterations)
{
	for (UINT32 i = pctx->first; i < iterations; i++) {
		sha256_Transform(pctx->idig, pctx->g, pctx->g);
		sha256_Transform(pctx->odig, pctx->g, pctx->g);
		for (UINT32 j = 0; j < SHA256_DIGEST_LENGTH / sizeof(UINT32); j++) {
			pctx->f[j] ^= pctx->g[j];
		}
	}
	pctx->first = 0;
}

void pbkdf2_hmac_sha256_Final(PBKDF2_HMAC_SHA256_CTX *pctx, BYTE *key)
{
#if BYTE_ORDER == LITTLE_ENDIAN
	for (UINT32 k = 0; k < SHA256_DIGEST_LENGTH / sizeof(UINT32); k++) {
		REVERSE32(pctx->f[k], pctx->f[k]);
	}
#endif
	memcpy(key, pctx->f, SHA256_DIGEST_LENGTH);
	memzero(pctx, sizeof(PBKDF2_HMAC_SHA256_CTX));
}

void pbkdf2_hmac_sha256(const BYTE *pass, int passlen, const BYTE *salt, int saltlen, UINT32 iterations, BYTE *key)
{
	PBKDF2_HMAC_SHA256_CTX pctx;
	pbkdf2_hmac_sha256_Init(&pctx, pass, passlen, salt, saltlen);
	pbkdf2_hmac_sha256_Update(&pctx, iterations);
	pbkdf2_hmac_sha256_Final(&pctx, key);
}

void pbkdf2_hmac_sha512_Init(PBKDF2_HMAC_SHA512_CTX *pctx, const BYTE *pass, int passlen, const BYTE *salt, int saltlen)
{
	SHA512_CTX ctx;
	UINT32 blocknr = 1;
#if BYTE_ORDER == LITTLE_ENDIAN
	REVERSE32(blocknr, blocknr);
#endif

	hmac_sha512_prepare(pass, passlen, pctx->odig, pctx->idig);
	memset(pctx->g, 0, sizeof(pctx->g));
	pctx->g[8] = 0x8000000000000000;
	pctx->g[15] = (SHA512_BLOCK_LENGTH + SHA512_DIGEST_LENGTH) * 8;

	memcpy(ctx.state, pctx->idig, sizeof(pctx->idig));
	ctx.bitcount[0] = SHA512_BLOCK_LENGTH * 8;
	ctx.bitcount[1] = 0;
	sha512_Update(&ctx, salt, saltlen);
	sha512_Update(&ctx, (BYTE*)&blocknr, sizeof(blocknr));
	sha512_Final(&ctx, (BYTE*)pctx->g);
#if BYTE_ORDER == LITTLE_ENDIAN
	for (UINT32 k = 0; k < SHA512_DIGEST_LENGTH / sizeof(UINT64); k++) {
		REVERSE64(pctx->g[k], pctx->g[k]);
	}
#endif
	sha512_Transform(pctx->odig, pctx->g, pctx->g);
	memcpy(pctx->f, pctx->g, SHA512_DIGEST_LENGTH);
	pctx->first = 1;
}

void pbkdf2_hmac_sha512_Update(PBKDF2_HMAC_SHA512_CTX *pctx, UINT32 iterations)
{
	for (UINT32 i = pctx->first; i < iterations; i++) {
		sha512_Transform(pctx->idig, pctx->g, pctx->g);
		sha512_Transform(pctx->odig, pctx->g, pctx->g);
		for (UINT32 j = 0; j < SHA512_DIGEST_LENGTH / sizeof(UINT64); j++) {
			pctx->f[j] ^= pctx->g[j];
		}
	}
	pctx->first = 0;
}

void pbkdf2_hmac_sha512_Final(PBKDF2_HMAC_SHA512_CTX *pctx, BYTE *key)
{
#if BYTE_ORDER == LITTLE_ENDIAN
	for (UINT32 k = 0; k < SHA512_DIGEST_LENGTH / sizeof(UINT64); k++) {
		REVERSE64(pctx->f[k], pctx->f[k]);
	}
#endif
	memcpy(key, pctx->f, SHA512_DIGEST_LENGTH);
	memzero(pctx, sizeof(PBKDF2_HMAC_SHA512_CTX));
}

void pbkdf2_hmac_sha512(const BYTE *pass, int passlen, const BYTE *salt, int saltlen, UINT32 iterations, BYTE *key)
{
	PBKDF2_HMAC_SHA512_CTX pctx;
	pbkdf2_hmac_sha512_Init(&pctx, pass, passlen, salt, saltlen);
	pbkdf2_hmac_sha512_Update(&pctx, iterations);
	pbkdf2_hmac_sha512_Final(&pctx, key);
}


