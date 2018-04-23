/*
 * keccak.c
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#include "keccak.h"
#include <string.h>


#define ROTL64(x, y)    (((x) << (y)) | ((x) >> (64 - (y))))
#define KECCAK_ROUNDS   24

const UINT64 keccakf_rndc[24] =
{
	0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
	0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
	0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
	0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
	0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
	0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
	0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
	0x8000000000008080, 0x0000000080000001, 0x8000000080008008,
};

const UINT64 keccakf_rotc[24] =
{
	1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14,
	27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44,
};

const UINT64 keccakf_piln[24] =
{
	10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
	15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1,
};

/* Updates the given state with a given number of rounds */
void keccakf(UINT64 _s[25], const int _rounds) {
	UINT64 i, j, round;
	register UINT64 t;
	UINT64 bc[5];

	for (round = 0; round < _rounds; round++) {
		//Theta step
		for (i = 0; i < 5; i++)
			bc[i] = _s[i] ^ _s[i + 5] ^ _s[i + 10] ^ _s[i + 15] ^ _s[i + 20];
		for (i = 0; i < 5; i++) {
			t = bc[(i + 4) % 5] ^ ROTL64(bc[(i + 1) % 5], 1);
			for (j = 0; j < 25; j += 5)
				_s[j + i] ^= t;
		}

		//Rho and Pi
		t = _s[1];
		for (i = 0; i < 24; i++) {
			j = keccakf_piln[i];
			bc[0] = _s[j];
			_s[j] = ROTL64(t, keccakf_rotc[i]);
			t = bc[0];
		}

		//Chi
		for (j = 0; j < 25; j += 5) {
			for (i = 0; i < 5; i++)
				bc[i] = _s[j + i];
			for (i = 0; i < 5; i++)
				_s[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
		}

		//Iota
		_s[0] ^= keccakf_rndc[round];
	}
}

void keccak(const BYTE *inData, const UINT32 lenInData, BYTE *hashOut) {
	UINT64 state[25];
	BYTE tmp[144];
	register UINT32 i, rsize, rsizew;
	register UINT32 j = lenInData;

	rsize = 200 - 2 * 32;
	rsizew = rsize / 8;

	//Clear the state
	memset(state, 0, sizeof(state));

	//First few blocks (i.e. not last block)
	for (; j >= rsize; j -= rsize, inData += rsize) {
		for (i = 0; i < rsizew; i++)
			state[i] ^= ((UINT64 *)inData)[i];
		keccakf(state, KECCAK_ROUNDS);
	}

	//Last block + padding
	memcpy(tmp, inData, j);
	tmp[j++] = 1;
	memset(tmp + j, 0, rsize - j);
	tmp[rsize - 1] |= 0x80;

	for (i = 0; i < rsizew; i++)
		state[i] ^= ((UINT64 *)tmp)[i];
	keccakf(state, KECCAK_ROUNDS);

	memcpy(hashOut, state, 32);
}
