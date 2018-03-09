/*
 * base58.c
 *
 *  Created on: Mar 5, 2018
 *      Author: User
 */
#include "base58.h"


void base58(BYTE *inData, int dataLen, BYTE *outData, int *outLen) {

	int len = 0;
	while ((int)inData[len] == 0)
	{
		len++;
	}

	int invertData[100] = { 0 };

	for (int i = 0; i < dataLen; i++)
	{
		invertData[i] = (int)inData[dataLen - 1 - i];
	}

	int coeff[201] = { 0 }; //массив коэффициентов для перевода 16-иричной системы в 10-ичную


	for (int i = 0; i < dataLen; i++)
	{
		coeff[2 * i + 1] = (invertData[i] - invertData[i] % 16) / 16;
		coeff[2 * i] = invertData[i] % 16;
	}

	int degree[201] = { 0 };	 //массив степеней числа 16

	int volue[201] = { 0 };    //массив значений в Base58

	degree[0] = 1;
	volue[0] = (degree[0] * coeff[0]) % 58;
	volue[1] = ((degree[0] * coeff[0]) - volue[0]) / 58;

	int temp[201] = { 0 }; //массив временных значений volue

	for (int j = 1; j < 200; j++)
	{
		for (int i = 0; i < 200; i++)
		{
			degree[i] *= 16;
		}
		for (int i = 0; i < 200; i++)
		{
			if (degree[i] > 57)
			{
				degree[i + 1] = degree[i + 1] + (degree[i] - degree[i] % 58) / 58;
				degree[i] = degree[i] % 58;
			}
		}
		for (int i = 0; i < 200; i++)
		{
			temp[i] = degree[i] * coeff[j];
		}
		for (int i = 0; i < 200; i++)
		{
			if (temp[i] > 57)
			{
				temp[i + 1] = temp[i + 1] + (temp[i] - temp[i] % 58) / 58;
				temp[i] = temp[i] % 58;
			}
		}
		for (int i = 0; i < 200; i++)
		{
			volue[i] = volue[i] + temp[i];
		}
	}

	for (int i = 0; i < 200; i++)
	{
		if (volue[i] > 57)
		{
			volue[i + 1] = volue[i + 1] + (volue[i] - volue[i] % 58) / 58;
			volue[i] = volue[i] % 58;
		}
	}

	int lenBase58 = 200;
	while (volue[lenBase58 - 1] == 0)
	{
		lenBase58--;
	}

	char invertBase58[201] = { 0 };

	char alfBase58[] = { "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz" };

	for (int i = 0; i < lenBase58; i++)
	{
		invertBase58[i] = alfBase58[(volue[i])];
	}

	char base58[201] = { 0 };


	for (int i = 0; i < lenBase58; i++)
	{
		base58[i] = invertBase58[lenBase58 - i - 1];

	}
	for (int i = 0; i < len; i++)
	{
		outData[i] = '1';
	}
	for (int i = len; i < len + lenBase58; i++)
	{
		outData[i] = base58[i - len];
	}
	*outLen = lenBase58 + len;
}


