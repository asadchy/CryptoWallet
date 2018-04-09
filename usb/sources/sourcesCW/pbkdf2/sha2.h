/*
 * sha2.h
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#ifndef SOURCES_SOURCESCW_PBKDF2_SHA2_H_
#define SOURCES_SOURCESCW_PBKDF2_SHA2_H_

#include <stdint.h>
#include <stddef.h>

typedef unsigned char BYTE;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

#define   SHA1_BLOCK_LENGTH		64
#define   SHA1_DIGEST_LENGTH		20
#define   SHA1_DIGEST_STRING_LENGTH	(SHA1_DIGEST_LENGTH   * 2 + 1)
#define SHA256_BLOCK_LENGTH		64
#define SHA256_DIGEST_LENGTH		32
#define SHA256_DIGEST_STRING_LENGTH	(SHA256_DIGEST_LENGTH * 2 + 1)
#define SHA512_BLOCK_LENGTH		128
#define SHA512_DIGEST_LENGTH		64
#define SHA512_DIGEST_STRING_LENGTH	(SHA512_DIGEST_LENGTH * 2 + 1)

typedef struct _SHA1_CTX {
	UINT32	state[5];
	UINT64	bitcount;
	UINT32	buffer[SHA1_BLOCK_LENGTH / sizeof(UINT32)];
} SHA1_CTX;
typedef struct _SHA256_CTX {
	UINT32	state[8];
	UINT64	bitcount;
	UINT32	buffer[SHA256_BLOCK_LENGTH / sizeof(UINT32)];
} SHA256_CTX;
typedef struct _SHA512_CTX {
	UINT64	state[8];
	UINT64	bitcount[2];
	UINT64	buffer[SHA512_BLOCK_LENGTH / sizeof(UINT64)];
} SHA512_CTX;

/*** ENDIAN REVERSAL MACROS *******************************************/
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN    4321
#endif

#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
#define REVERSE32(w,x)	{ \
	uint32_t tmp = (w); \
	tmp = (tmp >> 16) | (tmp << 16); \
	(x) = ((tmp & 0xff00ff00UL) >> 8) | ((tmp & 0x00ff00ffUL) << 8); \
}
#define REVERSE64(w,x)	{ \
	uint64_t tmp = (w); \
	tmp = (tmp >> 32) | (tmp << 32); \
	tmp = ((tmp & 0xff00ff00ff00ff00ULL) >> 8) | \
	      ((tmp & 0x00ff00ff00ff00ffULL) << 8); \
	(x) = ((tmp & 0xffff0000ffff0000ULL) >> 16) | \
	      ((tmp & 0x0000ffff0000ffffULL) << 16); \
}
#endif /* BYTE_ORDER == LITTLE_ENDIAN */

extern const UINT32 sha256_initial_hash_value[8];
extern const UINT64 sha512_initial_hash_value[8];

void sha1_Transform(const UINT32* state_in, const UINT32* data, UINT32* state_out);
void sha1_Init(SHA1_CTX *);
void sha1_Update(SHA1_CTX*, const BYTE*, UINT32);
void sha1_Final(SHA1_CTX*, BYTE[SHA1_DIGEST_LENGTH]);
char* sha1_End(SHA1_CTX*, char[SHA1_DIGEST_STRING_LENGTH]);
void sha1_Raw(const BYTE*, UINT32, BYTE[SHA1_DIGEST_LENGTH]);
char* sha1_Data(const BYTE*, UINT32, char[SHA1_DIGEST_STRING_LENGTH]);

void sha256_Transform(const UINT32* state_in, const UINT32* data, UINT32* state_out);
void sha256_Init(SHA256_CTX *);
void sha256_Update(SHA256_CTX*, const BYTE*, UINT32);
void sha256_Final(SHA256_CTX*, BYTE[SHA256_DIGEST_LENGTH]);
char* sha256_End(SHA256_CTX*, char[SHA256_DIGEST_STRING_LENGTH]);
void sha256_Raw(const BYTE*, UINT32, BYTE[SHA256_DIGEST_LENGTH]);
char* sha256_Data(const BYTE*, UINT32, char[SHA256_DIGEST_STRING_LENGTH]);

void sha512_Transform(const UINT64* state_in, const UINT64* data, UINT64* state_out);
void sha512_Init(SHA512_CTX*);
void sha512_Update(SHA512_CTX*, const BYTE*, UINT32);
void sha512_Final(SHA512_CTX*, BYTE[SHA512_DIGEST_LENGTH]);
char* sha512_End(SHA512_CTX*, char[SHA512_DIGEST_STRING_LENGTH]);
void sha512_Raw(const BYTE*, UINT32, BYTE[SHA512_DIGEST_LENGTH]);
char* sha512_Data(const BYTE*, UINT32, char[SHA512_DIGEST_STRING_LENGTH]);



#endif /* SOURCES_SOURCESCW_PBKDF2_SHA2_H_ */
