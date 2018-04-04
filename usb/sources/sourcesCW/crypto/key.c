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

void generatePrivateKey(int pin, int *idCurrency, BYTE *privKey) {
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
	/*
	BYTE msg[2] = { 0 };
	msg[0] = (unsigned char)(pin % 256);
	msg[1] = (unsigned char)((pin - (pin % 256)) / 256);
	for(int i = len; i < len + 2; i++)
	{
		tocen[i] = msg[i - len];
	}
	len = len + 2;
	HMAC_SHA256(privKey, seed, 32, tocen, len);
	*/

	BYTE msg[2] = { 0 };
	msg[0] = (unsigned char)(pin % 256);
	msg[1] = (unsigned char)((pin - (pin % 256)) / 256);

	BYTE temp[32] = { 0 };
	SHA_256(msg, 2, temp);
	HMAC_SHA256(privKey, temp, 32, tocen, len);
}


void genKeyC(int pin, int idCurrency, BYTE * privKey, BYTE * pubKey, int compressed) {
	BYTE pr[32] = { 0 };
	generatePrivateKey(pin, &idCurrency, pr);
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

void genKeyE(int pin, int idCurrency, BYTE * privKey, BYTE * pubKey) {

	generatePrivateKey(pin, &idCurrency, privKey);
	const struct uECC_Curve_t * curves;
	curves = uECC_secp256k1();
	uECC_compute_public_key(privKey, pubKey, curves);
}

