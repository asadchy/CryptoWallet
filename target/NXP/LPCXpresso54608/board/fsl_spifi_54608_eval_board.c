#include "board.h"
#include "fsl_spifi_dma.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "fsl_spifi_54608_eval_board.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t g_buffer[PAGE_SIZE] = {0};

static spifi_command_t command[COMMAND_NUM] = {
    {PAGE_SIZE, false, kSPIFI_DataInput, 1, kSPIFI_CommandDataQuad, kSPIFI_CommandOpcodeAddrThreeBytes, 0x6B},
    {PAGE_SIZE, false, kSPIFI_DataOutput, 0, kSPIFI_CommandOpcodeSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0x38},
    {4, false, kSPIFI_DataInput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x05},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0x20},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x06},
    {4, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x01}};

/*******************************************************************************
 * Code
 ******************************************************************************/
void check_if_finish()
{
    uint32_t val = 0;
    /* Check WIP bit */
    do
    {
        SPIFI_SetCommand(SPIFI, &command[GET_STATUS]);
        while ((SPIFI->STAT & SPIFI_STAT_INTRQ_MASK) == 0U)
        {
        }
        val = SPIFI_ReadData(SPIFI);
    } while (val & 0x1);
}

static void enable_quad_mode()
{
    /* Write enable */
    SPIFI_SetCommand(SPIFI, &command[WRITE_ENABLE]);

    /* Set write register command */
    SPIFI_SetCommand(SPIFI, &command[WRITE_REGISTER]);

    SPIFI_WriteData(SPIFI, 0x40);

    check_if_finish();
}

void BOARD_InitSPIFI(void)
{
    spifi_config_t config = {0};
    uint32_t sourceClockFreq;

    /* attach 12 MHz clock to SPI3 */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM9);
    
    /* Set SPIFI clock source */
    CLOCK_AttachClk(kMAIN_CLK_to_SPIFI_CLK);
    sourceClockFreq = CLOCK_GetSpifiClkFreq();

    /* Set the clock divider */
    CLOCK_SetClkDiv(kCLOCK_DivSpifiClk, sourceClockFreq / SPI_BAUDRATE, false);

    /* Initialize SPIFI */
    SPIFI_GetDefaultConfig(&config);
    config.isReadFullClockCycle = true;  //SSN changed this (was false). Required for 48MHz SPIFI.
    SPIFI_Init(SPIFI, &config);

    /* Enable Quad mode */
    enable_quad_mode();

    /* Setup memory command */
    SPIFI_SetMemoryCommand(SPIFI, &command[READ]);
}
