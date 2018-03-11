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
	/*BYTE n[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
				 0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B,
				 0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x41 };
			BYTE lo_s[32] = { 0 };
			if (!(outData[32] < 0x80)) {
				for (int i = 31; i > 0; i--)
				{
					if ((int)n[i]<(int)outData[i+32]) {
						lo_s[i] = 256 + (int)n[i] - (int)outData[i+32];
						n[i - 1]--;
					}
					else {
						lo_s[i] = (int)n[i] - (int)outData[i+32];
					}
				}
				lo_s[0] = (int)n[0] - (int)outData[32];
				for (int i = 0; i < 32; i++)
				{
					outData[i + 32] = lo_s[i];
				}
			}
	for(int i = 0; i<33; i++){
		outData[i+64] = publicKey[i];
	}*/

	//*lenOutData = 97;
}

void getSignEther(BYTE *privKey, BYTE* hash, BYTE* outData) {
	const struct uECC_Curve_t * curves;
	curves = uECC_secp256k1();
	BYTE pubKey[64] = { 0 };
	uECC_compute_public_key(privKey, pubKey, curves);

	uECC_sign_with_nonce(privKey, hash, 32, hash, outData, curves);

	outData[64] = 27 + (pubKey[63] & 1);
}
