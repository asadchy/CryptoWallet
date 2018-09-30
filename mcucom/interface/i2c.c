/*
 * i2c.c
 *
 *  Created on: Apr 19, 2018
 *      Author: artyom
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "data.hpp"
#include "i2c.h"
#include "fsl_i2c.h"
#include "string.h"

#define I2C_CLOCK_FREQUENCY			(12000000UL)
#define I2C_BASE					((I2C_Type *)I2C8_BASE)
#define I2C_IRQ						FLEXCOMM8_IRQn

i2c_slave_handle_t i2c_handle;
uint8_t empty_packet[IPC_BUF_SIZE];

static void i2c_callback(I2C_Type *base, volatile i2c_slave_transfer_t *xfer, void *user_data)
{
	struct ipc_wrapper *wrp = (struct ipc_wrapper*)user_data;

    switch (xfer->event)
    {
        /*  Address match event */
        case kI2C_SlaveAddressMatchEvent:
            xfer->rxData = NULL;
            xfer->rxSize = 0;
            //wrp->tx->ready = false;
            //wrp->rx->ready = false;
            break;

        /*  Transmit request */
        case kI2C_SlaveTransmitEvent:
            /*  Update information for transmit process */
        	if(wrp->tx->ready)
        	{
				xfer->txData = wrp->tx->data;
				xfer->txSize = wrp->tx->size;
				wrp->tx->ready = false;
        	}
#if 0
        	else
        	{
        		xfer->txData = empty_packet;
				xfer->txSize = IPC_BUF_SIZE;
        	}
#endif
            break;

        /* Setup the slave receive buffer */
        case kI2C_SlaveReceiveEvent:
            /*  Update information for received process */
				xfer->rxData = wrp->rx->data;
				xfer->rxSize = wrp->rx->size;
            break;

        /* The master has sent a stop transition on the bus */
        case kI2C_SlaveCompletionEvent:
            //xSemaphoreGiveFromISR(*(wrp->sem), NULL);
        	wrp->rx->ready = true;
            break;

        default:
            break;
    }
}

void BOARD_InitI2C(struct ipc_wrapper *wrp)
{
	i2c_slave_config_t config;
	status_t ret = kStatus_Fail;

	memset(empty_packet, 0, IPC_BUF_SIZE);

	/* attach 12 MHz clock to FLEXCOMM8 (I2C slave) */
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM8);

	/* reset FLEXCOMM for I2C */
	RESET_PeripheralReset(kFC8_RST_SHIFT_RSTn);

	/*  Set i2c slave interrupt priority higher. */
	//NVIC_SetPriority(I2C_IRQ, 3);
	//EnableIRQ(I2C_IRQ);

	/* Set up i2c slave */
	I2C_SlaveGetDefaultConfig(&config);

	/* Change the slave address */
	config.address0.address = I2C_ADDR;

	/* Initialize the I2C slave peripheral */
	I2C_SlaveInit(I2C_BASE, &config, I2C_CLOCK_FREQUENCY);

	/* Create the I2C handle for the non-blocking transfer */
	I2C_SlaveTransferCreateHandle(I2C_BASE, &i2c_handle, i2c_callback, (void*)wrp);

	/* Start accepting I2C transfers on the I2C slave peripheral */
	ret = I2C_SlaveTransferNonBlocking(I2C_BASE, &i2c_handle, kI2C_SlaveAddressMatchEvent | kI2C_SlaveCompletionEvent);

	if (ret != kStatus_Success)
	{
		while(1);
	}
}
