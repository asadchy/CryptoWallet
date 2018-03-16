/*
 * flash.c
 *
 *  Created on: Mar 15, 2018
 *      Author: User
 */

#include "flash.h"
#include "fsl_spifi_54608_eval_board.h"
#include "fsl_spifi.h"

//#ifdef gfhfgfhf
#define FLASH_ADDR			0x10FFF000UL

static spifi_command_t command[COMMAND_NUM] = {
    {PAGE_SIZE, false, kSPIFI_DataInput, 1, kSPIFI_CommandDataQuad, kSPIFI_CommandOpcodeAddrThreeBytes, 0x6B},
    {PAGE_SIZE, false, kSPIFI_DataOutput, 0, kSPIFI_CommandOpcodeSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0x38},
    {4, false, kSPIFI_DataInput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x05},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0x20},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x06},
    {4, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x01}};

void write_flash2(uint32_t *data, uint32_t size)
{
	if(size > PAGE_SIZE / 4)
		size = PAGE_SIZE / 4;

	//portENTER_CRITICAL();
	/* Reset the SPIFI to switch to command mode */
	SPIFI_ResetCommand(SPIFI);
	EnableIRQ(SPIFI0_IRQn);
	/* Write enable */
	SPIFI_SetCommand(SPIFI, &command[WRITE_ENABLE]);
	/* Set address */
	SPIFI_SetCommandAddress(SPIFI, FLASH_ADDR);
	/* Erase sector */
	SPIFI_SetCommand(SPIFI, &command[ERASE_SECTOR]);
	/* Check if finished */
	check_if_finish();

	SPIFI_SetCommand(SPIFI, &command[WRITE_ENABLE]);
	SPIFI_SetCommandAddress(SPIFI, FLASH_ADDR);
	SPIFI_SetCommand(SPIFI, &command[PROGRAM_PAGE]);
	for(int i = 0; i < size; i++)
	{
		SPIFI_WriteData(SPIFI, data[i]);
	}
	if(size < PAGE_SIZE / 4)
	{
		for(int i = size; i < PAGE_SIZE / 4; i++)
		{
			SPIFI_WriteData(SPIFI, 0);
		}
	}
	check_if_finish();

	SPIFI_ResetCommand(SPIFI);
	SPIFI_SetMemoryCommand(SPIFI, &command[READ]);
	//portEXIT_CRITICAL();
}

void read_flash2(uint32_t *data, uint32_t size)
{
	if(size > PAGE_SIZE / 4)
			size = PAGE_SIZE / 4;

	//portENTER_CRITICAL();
	/* Reset to memory command mode */
	SPIFI_ResetCommand(SPIFI);
	/* Read enable */
	SPIFI_SetMemoryCommand(SPIFI, &command[READ]);

	for(int i = 0; i < size; i++)
	{
		data[i] = *(uint32_t*)(FLASH_ADDR + i * sizeof(uint32_t));
	}
	//portEXIT_CRITICAL();
}

//#endif
