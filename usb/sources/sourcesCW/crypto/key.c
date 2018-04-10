/*
 * key.c
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */
#include "sha256.h"
#include "key.h"
#include "uECC.h"
#include "pbkdf2/mnemonic.h"

void generatePrivateKey(BYTE *seed, int *idCurrency, BYTE *privKey) {
	BYTE tocen[10] = { 0 };
	int len = 0;
	switch (*idCurrency)
	{
	case 0: {
		len = 7;
		tocen[0] = 'b';
		tocen[1] = 'i';
		tocen[2] = 't';
		tocen[3] = 'c';
		tocen[4] = 'o';
		tocen[5] = 'i';
		tocen[6] = 'n';
		break;
	}
	case 1: {
		len = 8;
		tocen[0] = 'e';
		tocen[1] = 't';
		tocen[2] = 'h';
		tocen[3] = 'e';
		tocen[4] = 'r';
		tocen[5] = 'e';
		tocen[6] = 'u';
		tocen[7] = 'm';
		break;
	}
	case 2: {
		len = 8;
		tocen[0] = 'l';
		tocen[1] = 'i';
		tocen[2] = 't';
		tocen[3] = 'e';
		tocen[4] = 'c';
		tocen[5] = 'o';
		tocen[6] = 'i';
		tocen[7] = 'n';
		break;
	}
	default:
		break;
	}
	BYTE temp[32] = { 0 };
	SHA_256(seed, 32, temp);
	HMAC_SHA256(privKey, temp, 32, tocen, len);
}


void genKeyC(uint32_t *pinDef, int idCurrency, BYTE * privKey, BYTE * pubKey, int compressed) {
	BYTE pr[32] = { 0 };
	uint8_t seed[32] = {0};
	for(int i = 0; i<32; i++)
	{
		seed[i] = pinDef[i+2];
	}
	generatePrivateKey(seed, &idCurrency, pr);
	const struct uECC_Curve_t * curves;
	curves = uECC_secp256k1();
	BYTE pub[64] = { 0 };
	uECC_compute_public_key(pr, pub, curves);
	if (compressed) {
		uECC_compress(pub, pubKey, curves);
	}
	else {
		pubKey[0] =  0x04;
		for (int i = 0; i < 64; i++)
		{
			pubKey[i + 1] = pub[i];
		}
	}
	for (int i = 0; i < 32; i++)
	{
		privKey[i] = pr[i];
	}

}

void genKeyE(uint32_t *pinDef, int idCurrency, BYTE * privKey, BYTE * pubKey) {
	uint8_t seed[32] = {0};
	for(int i = 0; i<32; i++)
	{
		seed[i] = pinDef[i+2];
	}
	generatePrivateKey(seed, &idCurrency, privKey);
	const struct uECC_Curve_t * curves;
	curves = uECC_secp256k1();
	uECC_compute_public_key(privKey, pubKey, curves);
}

