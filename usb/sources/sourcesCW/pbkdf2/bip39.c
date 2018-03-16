/*
 * bip39.c
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#include <string.h>
#include <stdbool.h>

#include "bip39.h"
#include "hmac.h"
#include "sha2.h"
#include "pbkdf2.h"
#include "bip39_english.h"
#include "memzero.h"



void charcpy(char*a, const char*b) {
	for (int i = 0; i < (int)strlen(b); i++)
	{
		a[i] = b[i];
		if (strlen(a)==i-1)
		{
			break;
		}
	}
}

const char *mnemonic_generate(int strength , BYTE* password)
{
	if (strength % 32 || strength < 128 || strength > 256) {
		return 0;
	}
	BYTE data[32] = { 0 };
	//random_buffer(data, 32);
	//for (int i = 0; i < 32;i++) data[i] = 0x01;


	sha256_Raw(password, 16, data);

	const char *r = mnemonic_from_data(data, strength / 8);
	memzero(data, sizeof(data));
	return r;
}

const char *mnemonic_from_data(const BYTE *data, int len)
{
	if (len % 4 || len < 16 || len > 32) {
		return 0;
	}

	BYTE bits[32 + 1];

	sha256_Raw(data, len, bits);
	// checksum
	bits[len] = bits[0];
	// data
	memcpy(bits, data, len);

	int mlen = len * 3 / 4;
	static char mnemo[24 * 10];

	int i, j, idx;
	char *p = mnemo;
	for (i = 0; i < mlen; i++) {
		idx = 0;
		for (j = 0; j < 11; j++) {
			idx <<= 1;
			idx += (bits[(i * 11 + j) / 8] & (1 << (7 - ((i * 11 + j) % 8)))) > 0;
		}
		charcpy(p, wordlist[idx]);

		p += strlen(wordlist[idx]);
		*p = (i < mlen - 1) ? ' ' : 0;
		p++;
	}
	memzero(bits, sizeof(bits));

	return mnemo;
}

const UINT16 *mnemonic_from_data_indexes(const BYTE *data, int len)
{
	if (len % 4 || len < 16 || len > 32) {
		return 0;
	}

	BYTE bits[32 + 1];

	sha256_Raw(data, len, bits);
	// checksum
	bits[len] = bits[0];
	// data
	memcpy(bits, data, len);

	int mlen = len * 3 / 4;
	static UINT16 mnemo[24];

	int i, j, idx;
	for (i = 0; i < mlen; i++) {
		idx = 0;
		for (j = 0; j < 11; j++) {
			idx <<= 1;
			idx += (bits[(i * 11 + j) / 8] & (1 << (7 - ((i * 11 + j) % 8)))) > 0;
		}
		mnemo[i] = idx;
	}
	memzero(bits, sizeof(bits));

	return mnemo;
}

int mnemonic_check(const char *mnemonic)
{
	if (!mnemonic) {
		return 0;
	}

	UINT32 i, n;

	i = 0; n = 0;
	while (mnemonic[i]) {
		if (mnemonic[i] == ' ') {
			n++;
		}
		i++;
	}
	n++;
	// check number of words
	if (n != 12 && n != 18 && n != 24) {
		return 0;
	}

	char current_word[10];
	UINT32 j, k, ki, bi;
	BYTE bits[32 + 1];

	memzero(bits, sizeof(bits));
	i = 0; bi = 0;
	while (mnemonic[i]) {
		j = 0;
		while (mnemonic[i] != ' ' && mnemonic[i] != 0) {
			if (j >= sizeof(current_word) - 1) {
				return 0;
			}
			current_word[j] = mnemonic[i];
			i++; j++;
		}
		current_word[j] = 0;
		if (mnemonic[i] != 0) i++;
		k = 0;
		for (;;) {
			if (!wordlist[k]) { // word not found
				return 0;
			}
			if (strcmp(current_word, wordlist[k]) == 0) { // word found on index k
				for (ki = 0; ki < 11; ki++) {
					if (k & (1 << (10 - ki))) {
						bits[bi / 8] |= 1 << (7 - (bi % 8));
					}
					bi++;
				}
				break;
			}
			k++;
		}
	}
	if (bi != n * 11) {
		return 0;
	}
	bits[32] = bits[n * 4 / 3];
	sha256_Raw(bits, n * 4 / 3, bits);
	if (n == 12) {
		return (bits[0] & 0xF0) == (bits[32] & 0xF0); // compare first 4 bits
	}
	else
		if (n == 18) {
			return (bits[0] & 0xFC) == (bits[32] & 0xFC); // compare first 6 bits
		}
		else
			if (n == 24) {
				return bits[0] == bits[32]; // compare 8 bits
			}
	return 0;
}

// passphrase must be at most 256 characters or code may crash
void mnemonic_to_seed(const char *mnemonic, const char *passphrase, BYTE seed[512 / 8])
{
	int passphraselen = strlen(passphrase);
	BYTE salt[8 + 256];
	memcpy(salt, "mnemonic", 8);
	memcpy(salt + 8, passphrase, passphraselen);
	 PBKDF2_HMAC_SHA512_CTX pctx;
	pbkdf2_hmac_sha512_Init(&pctx, (const BYTE *)mnemonic, strlen(mnemonic), salt, passphraselen + 8);
for (int i = 0; i < 16; i++) {
		pbkdf2_hmac_sha512_Update(&pctx, BIP39_PBKDF2_ROUNDS / 16);
		}
	pbkdf2_hmac_sha512_Final(&pctx, seed);
	memzero(salt, sizeof(salt));
}

const char * const *mnemonic_wordlist(void)
{
	return wordlist;
}


