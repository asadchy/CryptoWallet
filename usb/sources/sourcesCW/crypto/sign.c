/*
 * sign.c
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#include "sign.h"
#include "uECC.h"
#include "sha256.h"
#include "transformSign.h"


void getSignBtc(BYTE *privKey, int compressed, BYTE* hash, BYTE* outData, int *lenOutData) {
	const struct uECC_Curve_t * curves;
	curves = uECC_secp256k1();
	BYTE publicKey[64] = { 0 };
	uECC_compute_public_key(privKey, publicKey, curves);

		for (int i = 0; i<32; i++){
			publicKey[32-i] = publicKey[31-i];
		}
		publicKey[0] = 2 + (publicKey[63]&0x01);

	uECC_sign_with_nonce(privKey, hash, 32, hash, outData, curves);
	transfomSig(outData, publicKey, lenOutData);

}

void getSignEther(BYTE *privKey, BYTE* hash, BYTE* outData) {
	const struct uECC_Curve_t * curves;
	curves = uECC_secp256k1();
	BYTE pubKey[64] = { 0 };
	uECC_compute_public_key(privKey, pubKey, curves);

	uECC_sign_with_nonce(privKey, hash, 32, hash, outData, curves);

	outData[64] = 27 + (pubKey[63] & 1);
}
