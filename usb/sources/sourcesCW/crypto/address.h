/*
 * address.h
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */

#ifndef SOURCESCW_CRYPTO_ADDRESS_H_
#define SOURCESCW_CRYPTO_ADDRESS_H_
#pragma once
typedef unsigned char BYTE;


/*
(IN) BYTE* xPubKeyBin - public key
(IN) bool compressed - true (compressed public key 33byte) / false (uncompressed public key 65byte)
(IN) bool net - true: Pubkey hash (P2PKH address) / false: Testnet pubkey hash
(OUT) BYTE *outAdrress - Pubkey hash (P2PKH address) !!![min34]!!!
(OUT) int outAddressLength - length addres
*/
void addressBtc(BYTE *PubKeyBin, int compressed, BYTE *outAddress, int *outAddressLength);

void addressLtc(BYTE *PubKeyBin, int compressed, BYTE *outAddress, int *outAddressLength);

void addressEth(BYTE *pubKeyBin, BYTE *outAddress);


#endif /* SOURCESCW_CRYPTO_ADDRESS_H_ */
