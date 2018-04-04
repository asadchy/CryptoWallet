/*
 * mnemonic.h
 *
 *  Created on: Mar 16, 2018
 *      Author: User
 */

#ifndef SOURCES_SOURCESCW_PBKDF2_MNEMONIC_H_
#define SOURCES_SOURCESCW_PBKDF2_MNEMONIC_H_

typedef unsigned char BYTE;
typedef unsigned int UINT32;

void mnemonicGenerate(BYTE *password, BYTE* mnemonic, int *lenMnem, int strength);


void seedFromMnemonic(BYTE *mnemonic, UINT32 lenMnem, BYTE *password, UINT32 lenPass, BYTE *seed);


#endif /* SOURCES_SOURCESCW_PBKDF2_MNEMONIC_H_ */
