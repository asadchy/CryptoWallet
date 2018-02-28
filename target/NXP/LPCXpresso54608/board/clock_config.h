#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL0_CLK_HZ                         12000000U  /*!< Board xtal0 frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ                          32768U  /*!< Board xtal32K frequency in Hz */

/*******************************************************************************
 ********************* Configuration BOARD_BootClock180M ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClock180M configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCK180M_CORE_CLOCK   180000000U    /*!< Core clock frequency:180000000Hz */

/*******************************************************************************
 * API for BOARD_BootClock180M configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClock180M(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ******************* Configuration BOARD_BootClockFROHF48M *********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockFROHF48M configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKFROHF48M_CORE_CLOCK   48000000U    /*!< Core clock frequency:48000000Hz */

/*******************************************************************************
 * API for BOARD_BootClockFROHF48M configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockFROHF48M(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ******************* Configuration BOARD_BootClockFROHF96M *********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockFROHF96M configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKFROHF96M_CORE_CLOCK   96000000U    /*!< Core clock frequency:96000000Hz */

/*******************************************************************************
 * API for BOARD_BootClockFROHF96M configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockFROHF96M(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************** Configuration BOARD_BootClock12M ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClock12M configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCK12M_CORE_CLOCK   12000000U    /*!< Core clock frequency:12000000Hz */

/*******************************************************************************
 * API for BOARD_BootClock12M configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClock12M(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */

