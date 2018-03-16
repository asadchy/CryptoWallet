/*
 * mnemonic.c
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#include "mnemonic.h"
#include "bip39.h"
#include "sha2.h"
#include <string.h>

void mnemonicGenerate(BYTE *password, BYTE* mnemonic, int *lenMnem, int strength)
{
	const char *mnem = mnemonic_generate(strength, password);
	*lenMnem = strlen(mnem);
	for (int i = 0; i < *lenMnem; i++)
	{
		mnemonic[i] = mnem[i];
	}

}

void seedFromMnemonic(BYTE *mnemonic, UINT32 lenMnem, BYTE *password, UINT32 lenPass, BYTE *seed)
{
	BYTE seedT[64] = {0};
	mnemonic_to_seed((char*)mnemonic, lenMnem, (char*)password, lenPass, seedT);
	sha256_Raw(seedT, 64, seed);
}
