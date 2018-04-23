/*
 * i2c.h
 *
 *  Created on: Apr 19, 2018
 *      Author: artyom
 */

#ifndef I2C_H_
#define I2C_H_

#include "stdint.h"
#include "stdbool.h"
#include "semphr.h"
#include "data.hpp"

#define I2C_ADDR		0x7E

void BOARD_InitI2C(struct ipc_wrapper *wrp);

#endif /* I2C_H_ */
