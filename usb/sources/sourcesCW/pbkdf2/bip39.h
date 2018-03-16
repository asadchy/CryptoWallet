/*
 * bip39.h
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#ifndef SOURCES_SOURCESCW_PBKDF2_BIP39_H_
#define SOURCES_SOURCESCW_PBKDF2_BIP39_H_

typedef unsigned char BYTE;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

#include <stdint.h>

#define BIP39_PBKDF2_ROUNDS 2048

const char *mnemonic_generate(int strength, BYTE* password);	// strength in bits, password[32]


const char *mnemonic_from_data(const BYTE *data, int len);
const UINT16 *mnemonic_from_data_indexes(const BYTE *data, int len);

int mnemonic_check(const char *mnemonic);

// passphrase must be at most 256 characters or code may crash
void mnemonic_to_seed(const char *mnemonic, UINT32 lenMnem, const char *passphrase, UINT32 lenPass, BYTE seed[512 / 8]);

const char * const *mnemonic_wordlist(void);


#endif /* SOURCES_SOURCESCW_PBKDF2_BIP39_H_ */
