/*
 * memzero.c
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#include <string.h>
#include "memzero.h"

void memzero(void *s, UINT32 n)
{
	memset(s, 0, n);
}


