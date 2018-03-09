/*
 * sha256.c
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */
#include "sha256.h"


#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

typedef struct {
	BYTE data[64];
	UINT32 datalen;
	UINT32 bitlen[2];
	UINT32 state[8];
} SHA256_CTX;

UINT32 k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};


void SHA256Transform(SHA256_CTX *ctx, BYTE data[])
{
	UINT32 a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for (; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void SHA256Init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen[0] = 0;
	ctx->bitlen[1] = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void SHA256Update(SHA256_CTX *ctx, BYTE data[], UINT32 len)
{
	for (UINT32 i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			SHA256Transform(ctx, ctx->data);
			DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], 512);
			ctx->datalen = 0;
		}
	}
}

void SHA256Final(SHA256_CTX *ctx, BYTE hash[])
{
	UINT32 i = ctx->datalen;

	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;

		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;

		while (i < 64)
			ctx->data[i++] = 0x00;

		SHA256Transform(ctx, ctx->data);
		//memset(ctx->data, 0, 56);
		for(int i=0; i<56;i++){
			ctx->data[i] = 0;
		}
	}

	DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], ctx->datalen * 8);
	ctx->data[63] = ctx->bitlen[0];
	ctx->data[62] = ctx->bitlen[0] >> 8;
	ctx->data[61] = ctx->bitlen[0] >> 16;
	ctx->data[60] = ctx->bitlen[0] >> 24;
	ctx->data[59] = ctx->bitlen[1];
	ctx->data[58] = ctx->bitlen[1] >> 8;
	ctx->data[57] = ctx->bitlen[1] >> 16;
	ctx->data[56] = ctx->bitlen[1] >> 24;
	SHA256Transform(ctx, ctx->data);

	for (i = 0; i < 4; ++i) {
		hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

void SHA_256(BYTE* data, UINT32 sizeData, BYTE* outSha256) {
	SHA256_CTX ctx;
	BYTE hash[32] = { 0 };
	SHA256Init(&ctx);
	SHA256Update(&ctx, data, sizeData);
	SHA256Final(&ctx, hash);

	for (int i = 0; i < 32; i++)
	{
		outSha256[i] = hash[i];
	}
}

void DoubleSHA_256(BYTE* data, UINT32 sizeData, BYTE* outSha256) {
	SHA256_CTX ctx;
	BYTE hash[32] = { 0 };
	SHA256Init(&ctx);
	SHA256Update(&ctx, data, sizeData);
	SHA256Final(&ctx, hash);
	BYTE doubleHash[32] = { 0 };
	SHA_256(hash, 32, doubleHash);

	for (int i = 0; i < 32; i++)
	{
		outSha256[i] = doubleHash[i];
	}
}

void HMAC_SHA256(BYTE out[], BYTE *data, UINT32 data_len, BYTE *key, UINT32 key_len)
{
#define B 64
#define L (SHA256_DIGEST_SIZE)
#define K (SHA256_DIGEST_SIZE * 2)

#define I_PAD 0x36
#define O_PAD 0x5C
	SHA256_CTX ss;
	BYTE kh[32] = { 0 };

	/*
	* If the key length is bigger than the buffer size B, apply the hash
	* function to it first and use the result instead.
	*/
	if (key_len > B) {
		SHA256Init(&ss);
		SHA256Update(&ss, key, key_len);
		SHA256Final(&ss, kh);
		key_len = 32;
		key = kh;
	}

	/*
	* (1) append zeros to the end of K to create a B byte string
	*     (e.g., if K is of length 20 bytes and B=64, then K will be
	*     appended with 44 zero bytes 0x00)
	* (2) XOR (bitwise exclusive-OR) the B byte string computed in step
	*     (1) with ipad
	*/
	BYTE kx[B];
	for (UINT64 i = 0; i < key_len; i++) kx[i] = I_PAD ^ key[i];
	for (UINT64 i = key_len; i < B; i++) kx[i] = I_PAD ^ 0;

	/*
	* (3) append the stream of data 'text' to the B byte string resulting
	*     from step (2)
	* (4) apply H to the stream generated in step (3)
	*/
	SHA256Init(&ss);
	SHA256Update(&ss, kx, B);
	SHA256Update(&ss, data, data_len);
	SHA256Final(&ss, out);

	/*
	* (5) XOR (bitwise exclusive-OR) the B byte string computed in
	*     step (1) with opad
	*
	* NOTE: The "kx" variable is reused.
	*/
	for (UINT64 i = 0; i < key_len; i++) kx[i] = O_PAD ^ key[i];
	for (UINT64 i = key_len; i < B; i++) kx[i] = O_PAD ^ 0;

	/*
	* (6) append the H result from step (4) to the B byte string
	*     resulting from step (5)
	* (7) apply H to the stream generated in step (6) and output
	*     the result
	*/
	SHA256Init(&ss);
	SHA256Update(&ss, kx, B);
	SHA256Update(&ss, out, 32);
	SHA256Final(&ss, out);
}

