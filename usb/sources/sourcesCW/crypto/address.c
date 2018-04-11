/*
 * address.c
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */
#include "base58.h"
#include "sha256.h"
#include "ripemd160.h"
#include "address.h"
#include "keccak.h"


void addressBtc(BYTE *PubKeyBin, int compressed, BYTE *outAddress, int *outAddressLength)
{
	UINT32 len = 65;
	if (compressed == 1) {
		len = 33;
	}
	else {
		len = 65;
	}

	BYTE outSha256[32] = { 0 };
	SHA_256(PubKeyBin, len, outSha256);

	len = 32;
	BYTE outRIPEMD160[20] = { 0 };
	ripemd160(outSha256, len, outRIPEMD160);

	/////////////////////////////////////////////////////////////////////////////////////////
	BYTE add00ByteRIPEMD160[21] = { 0 };
	add00ByteRIPEMD160[0] =  0x6f; //testnet 0x6f, real 0x00
	for (int i = 0; i < 20; i++)
	{
		add00ByteRIPEMD160[i + 1] = outRIPEMD160[i];
	}
	len = 21;

	BYTE Sha256Sha256Add00Byte[32] = { 0 };

	DoubleSHA_256(add00ByteRIPEMD160, len, Sha256Sha256Add00Byte);
	BYTE PubKey25Byte[25] = { 0 };
	for (int i = 0; i < 21; i++)
	{
		PubKey25Byte[i] = add00ByteRIPEMD160[i];
	}
	for (int i = 21; i < 25; i++)
	{
		PubKey25Byte[i] = Sha256Sha256Add00Byte[i - 21];
	}

int	len1 = 25;
uint8_t add[50] = {0};
int addlen = 0;
	base58(PubKey25Byte, len1, add, &addlen);
/*for(int i = 0; i<addlen; i++){
	outAddress[i] = add[i];
}
*outAddressLength =addlen;

*/

for(int i = 0; i<addlen; i++){
		outAddress[i] = add[i];
	}
	*outAddressLength = addlen;
}




void addressLtc(BYTE *PubKeyBin, int compressed, BYTE *outAddress, int *outAddressLength)
{
	int len = 65;
	if (compressed == 1) {
		len = 33;
	}
	else {
		len = 65;
	}

	BYTE outSha256[32] = { 0 };
	SHA_256(PubKeyBin, len, outSha256);

	len = 32;
	BYTE outRIPEMD160[20] = { 0 };
	ripemd160(outSha256, len, outRIPEMD160);

	/////////////////////////////////////////////////////////////////////////////////////////
	BYTE add00ByteRIPEMD160[21] = { 0 };
	add00ByteRIPEMD160[0] = 0x6f; //testnet 0x6f , real 0x30
	for (int i = 0; i < 20; i++)
	{
		add00ByteRIPEMD160[i + 1] = outRIPEMD160[i];
	}
	len = 21;
	BYTE Sha256Add00Byte[32] = { 0 };
	SHA_256(add00ByteRIPEMD160, len, Sha256Add00Byte);

	len = 32;
	BYTE Sha256Sha256Add00Byte[32] = { 0 };
	SHA_256(Sha256Add00Byte, len, Sha256Sha256Add00Byte);

	BYTE PubKey25Byte[25] = { 0 };
	for (int i = 0; i < 21; i++)
	{
		PubKey25Byte[i] = add00ByteRIPEMD160[i];
	}
	for (int i = 21; i < 25; i++)
	{
		PubKey25Byte[i] = Sha256Sha256Add00Byte[i - 21];
	}

	int	len1 = 25;
	uint8_t add[50] = {0};
	int addlen = 0;
		base58(PubKey25Byte, len1, add, &addlen);
	/*for(int i = 0; i<addlen; i++){
		outAddress[i] = add[i];
	}
	*outAddressLength =addlen;

	*/

	for(int i = 0; i<addlen; i++){
			outAddress[i] = add[i];
		}
		*outAddressLength = addlen;
}

void addressEth(BYTE *pubKeyBin, BYTE *outAddress)
{
	BYTE hash[32] = { 0 };
	keccak(pubKeyBin, 64, hash);
	for (int i = 0; i < 20; i++)
	{
		outAddress[i] = hash[i + 12];
	}
}

