#ifndef _FSL_FT5406_H_
#define _FSL_FT5406_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SPIFI SPIFI0
#define PAGE_SIZE (256)
#define SECTOR_SIZE (4096)
#define SPI_BAUDRATE (90000000)
#define COMMAND_NUM (6)
#define READ (0)
#define PROGRAM_PAGE (1)
#define GET_STATUS (2)
#define ERASE_SECTOR (3)
#define WRITE_ENABLE (4)
#define WRITE_REGISTER (5)

void BOARD_InitSPIFI(void);
void check_if_finish(void);

#endif
