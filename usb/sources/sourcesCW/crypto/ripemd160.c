/*
 * ripemd160.c
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#include "ripemd160.h"

typedef struct
{
	UINT32 total[2];            /*!< number of bytes processed  */
	UINT32 state[5];          /*!< intermediate digest state  */
	BYTE buffer[64];   /*!< data block being processed */

}
ripemd160_context;

/*
* 32-bit integer manipulation macros (little endian)
*/
#define GET_UINT32_LE(n,b,i)                          \
{                                                     \
    (n) = ( (UINT32) (b)[(i)    ]       )             \
        | ( (UINT32) (b)[(i) + 1] <<  8 )             \
        | ( (UINT32) (b)[(i) + 2] << 16 )             \
        | ( (UINT32) (b)[(i) + 3] << 24 );            \
}

#define PUT_UINT32_LE(n,b,i)                   \
{                                              \
    (b)[(i)    ] = (BYTE) ( (n)       );       \
    (b)[(i) + 1] = (BYTE) ( (n) >>  8 );       \
    (b)[(i) + 2] = (BYTE) ( (n) >> 16 );       \
    (b)[(i) + 3] = (BYTE) ( (n) >> 24 );       \
}

/*
* RIPEMD-160 context setup
*/
void ripemd160_starts(ripemd160_context *ctx)
{
	ctx->total[0] = 0;
	ctx->total[1] = 0;

	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
	ctx->state[4] = 0xC3D2E1F0;
}

/*
* Process one block
*/
void ripemd160_process(ripemd160_context *ctx, const BYTE data[64])
{
	UINT32 A, B, C, D, E, Ap, Bp, Cp, Dp, Ep, X[16];

	GET_UINT32_LE(X[0], data, 0);
	GET_UINT32_LE(X[1], data, 4);
	GET_UINT32_LE(X[2], data, 8);
	GET_UINT32_LE(X[3], data, 12);
	GET_UINT32_LE(X[4], data, 16);
	GET_UINT32_LE(X[5], data, 20);
	GET_UINT32_LE(X[6], data, 24);
	GET_UINT32_LE(X[7], data, 28);
	GET_UINT32_LE(X[8], data, 32);
	GET_UINT32_LE(X[9], data, 36);
	GET_UINT32_LE(X[10], data, 40);
	GET_UINT32_LE(X[11], data, 44);
	GET_UINT32_LE(X[12], data, 48);
	GET_UINT32_LE(X[13], data, 52);
	GET_UINT32_LE(X[14], data, 56);
	GET_UINT32_LE(X[15], data, 60);

	A = Ap = ctx->state[0];
	B = Bp = ctx->state[1];
	C = Cp = ctx->state[2];
	D = Dp = ctx->state[3];
	E = Ep = ctx->state[4];

#define F1( x, y, z )   ( x ^ y ^ z )
#define F2( x, y, z )   ( ( x & y ) | ( ~x & z ) )
#define F3( x, y, z )   ( ( x | ~y ) ^ z )
#define F4( x, y, z )   ( ( x & z ) | ( y & ~z ) )
#define F5( x, y, z )   ( x ^ ( y | ~z ) )

#define S( x, n ) ( ( x << n ) | ( x >> (32 - n) ) )

#define P( a, b, c, d, e, r, s, f, k )      \
    a += f( b, c, d ) + X[r] + k;           \
    a = S( a, s ) + e;                      \
    c = S( c, 10 );

#define P2( a, b, c, d, e, r, s, rp, sp )   \
    P( a, b, c, d, e, r, s, F, K );         \
    P( a ## p, b ## p, c ## p, d ## p, e ## p, rp, sp, Fp, Kp );

#define F   F1
#define K   0x00000000
#define Fp  F5
#define Kp  0x50A28BE6
	P2(A, B, C, D, E, 0, 11, 5, 8);
	P2(E, A, B, C, D, 1, 14, 14, 9);
	P2(D, E, A, B, C, 2, 15, 7, 9);
	P2(C, D, E, A, B, 3, 12, 0, 11);
	P2(B, C, D, E, A, 4, 5, 9, 13);
	P2(A, B, C, D, E, 5, 8, 2, 15);
	P2(E, A, B, C, D, 6, 7, 11, 15);
	P2(D, E, A, B, C, 7, 9, 4, 5);
	P2(C, D, E, A, B, 8, 11, 13, 7);
	P2(B, C, D, E, A, 9, 13, 6, 7);
	P2(A, B, C, D, E, 10, 14, 15, 8);
	P2(E, A, B, C, D, 11, 15, 8, 11);
	P2(D, E, A, B, C, 12, 6, 1, 14);
	P2(C, D, E, A, B, 13, 7, 10, 14);
	P2(B, C, D, E, A, 14, 9, 3, 12);
	P2(A, B, C, D, E, 15, 8, 12, 6);
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F2
#define K   0x5A827999
#define Fp  F4
#define Kp  0x5C4DD124
	P2(E, A, B, C, D, 7, 7, 6, 9);
	P2(D, E, A, B, C, 4, 6, 11, 13);
	P2(C, D, E, A, B, 13, 8, 3, 15);
	P2(B, C, D, E, A, 1, 13, 7, 7);
	P2(A, B, C, D, E, 10, 11, 0, 12);
	P2(E, A, B, C, D, 6, 9, 13, 8);
	P2(D, E, A, B, C, 15, 7, 5, 9);
	P2(C, D, E, A, B, 3, 15, 10, 11);
	P2(B, C, D, E, A, 12, 7, 14, 7);
	P2(A, B, C, D, E, 0, 12, 15, 7);
	P2(E, A, B, C, D, 9, 15, 8, 12);
	P2(D, E, A, B, C, 5, 9, 12, 7);
	P2(C, D, E, A, B, 2, 11, 4, 6);
	P2(B, C, D, E, A, 14, 7, 9, 15);
	P2(A, B, C, D, E, 11, 13, 1, 13);
	P2(E, A, B, C, D, 8, 12, 2, 11);
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F3
#define K   0x6ED9EBA1
#define Fp  F3
#define Kp  0x6D703EF3
	P2(D, E, A, B, C, 3, 11, 15, 9);
	P2(C, D, E, A, B, 10, 13, 5, 7);
	P2(B, C, D, E, A, 14, 6, 1, 15);
	P2(A, B, C, D, E, 4, 7, 3, 11);
	P2(E, A, B, C, D, 9, 14, 7, 8);
	P2(D, E, A, B, C, 15, 9, 14, 6);
	P2(C, D, E, A, B, 8, 13, 6, 6);
	P2(B, C, D, E, A, 1, 15, 9, 14);
	P2(A, B, C, D, E, 2, 14, 11, 12);
	P2(E, A, B, C, D, 7, 8, 8, 13);
	P2(D, E, A, B, C, 0, 13, 12, 5);
	P2(C, D, E, A, B, 6, 6, 2, 14);
	P2(B, C, D, E, A, 13, 5, 10, 13);
	P2(A, B, C, D, E, 11, 12, 0, 13);
	P2(E, A, B, C, D, 5, 7, 4, 7);
	P2(D, E, A, B, C, 12, 5, 13, 5);
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F4
#define K   0x8F1BBCDC
#define Fp  F2
#define Kp  0x7A6D76E9
	P2(C, D, E, A, B, 1, 11, 8, 15);
	P2(B, C, D, E, A, 9, 12, 6, 5);
	P2(A, B, C, D, E, 11, 14, 4, 8);
	P2(E, A, B, C, D, 10, 15, 1, 11);
	P2(D, E, A, B, C, 0, 14, 3, 14);
	P2(C, D, E, A, B, 8, 15, 11, 14);
	P2(B, C, D, E, A, 12, 9, 15, 6);
	P2(A, B, C, D, E, 4, 8, 0, 14);
	P2(E, A, B, C, D, 13, 9, 5, 6);
	P2(D, E, A, B, C, 3, 14, 12, 9);
	P2(C, D, E, A, B, 7, 5, 2, 12);
	P2(B, C, D, E, A, 15, 6, 13, 9);
	P2(A, B, C, D, E, 14, 8, 9, 12);
	P2(E, A, B, C, D, 5, 6, 7, 5);
	P2(D, E, A, B, C, 6, 5, 10, 15);
	P2(C, D, E, A, B, 2, 12, 14, 8);
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F5
#define K   0xA953FD4E
#define Fp  F1
#define Kp  0x00000000
	P2(B, C, D, E, A, 4, 9, 12, 8);
	P2(A, B, C, D, E, 0, 15, 15, 5);
	P2(E, A, B, C, D, 5, 5, 10, 12);
	P2(D, E, A, B, C, 9, 11, 4, 9);
	P2(C, D, E, A, B, 7, 6, 1, 12);
	P2(B, C, D, E, A, 12, 8, 5, 5);
	P2(A, B, C, D, E, 2, 13, 8, 14);
	P2(E, A, B, C, D, 10, 12, 7, 6);
	P2(D, E, A, B, C, 14, 5, 6, 8);
	P2(C, D, E, A, B, 1, 12, 2, 13);
	P2(B, C, D, E, A, 3, 13, 13, 6);
	P2(A, B, C, D, E, 8, 14, 14, 5);
	P2(E, A, B, C, D, 11, 11, 0, 15);
	P2(D, E, A, B, C, 6, 8, 3, 13);
	P2(C, D, E, A, B, 15, 5, 9, 11);
	P2(B, C, D, E, A, 13, 6, 11, 11);
#undef F
#undef K
#undef Fp
#undef Kp

	C = ctx->state[1] + C + Dp;
	ctx->state[1] = ctx->state[2] + D + Ep;
	ctx->state[2] = ctx->state[3] + E + Ap;
	ctx->state[3] = ctx->state[4] + A + Bp;
	ctx->state[4] = ctx->state[0] + B + Cp;
	ctx->state[0] = C;
}

/*
* RIPEMD-160 process buffer
*/
void ripemd160_update(ripemd160_context *ctx,
	const BYTE *input, UINT32 ilen)
{
	UINT32 fill;
	UINT32 left;

	if (ilen <= 0)
		return;

	left = ctx->total[0] & 0x3F;
	fill = 64 - left;

	ctx->total[0] += (UINT32)ilen;
	ctx->total[0] &= 0xFFFFFFFF;

	if (ctx->total[0] < (UINT32)ilen)
		ctx->total[1]++;

	if (left && ilen >= fill)
	{
	//	memcpy((void *)(ctx->buffer + left), input, fill);
		for(int i = 0; i < fill; i++){
			ctx->buffer[i+left]=input[i];
		}
		ripemd160_process(ctx, ctx->buffer);
		input += fill;
		ilen -= fill;
		left = 0;
	}

	while (ilen >= 64)
	{
		ripemd160_process(ctx, input);
		input += 64;
		ilen -= 64;
	}

	if (ilen > 0)
	{
		//memcpy((void *)(ctx->buffer + left), input, ilen);
		for(int i = 0; i < ilen; i++){
			ctx->buffer[i+left]=input[i];
		}
	}
}

static const BYTE ripemd160_padding[64] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
* RIPEMD-160 final digest
*/
void ripemd160_finish(ripemd160_context *ctx, BYTE output[20])
{
	UINT32 last, padn;
	UINT32 high, low;
	BYTE msglen[8];

	high = (ctx->total[0] >> 29)
		| (ctx->total[1] << 3);
	low = (ctx->total[0] << 3);

	PUT_UINT32_LE(low, msglen, 0);
	PUT_UINT32_LE(high, msglen, 4);

	last = ctx->total[0] & 0x3F;
	padn = (last < 56) ? (56 - last) : (120 - last);

	ripemd160_update(ctx, ripemd160_padding, padn);
	ripemd160_update(ctx, msglen, 8);

	PUT_UINT32_LE(ctx->state[0], output, 0);
	PUT_UINT32_LE(ctx->state[1], output, 4);
	PUT_UINT32_LE(ctx->state[2], output, 8);
	PUT_UINT32_LE(ctx->state[3], output, 12);
	PUT_UINT32_LE(ctx->state[4], output, 16);
}

/*
* output = RIPEMD-160( input buffer )
*/
void ripemd160(const BYTE *inData, UINT32 inDataLength, BYTE* outDataHesh)
{
	BYTE hesh[50] = {0};
	ripemd160_context ctx;
	ripemd160_starts(&ctx);
	ripemd160_update(&ctx, inData, inDataLength);
	ripemd160_finish(&ctx, hesh);
	//memset(&ctx, 0, sizeof(ripemd160_context));
	for(int i=0; i<20;i++){
		outDataHesh[i] = hesh[i];
	}
}
