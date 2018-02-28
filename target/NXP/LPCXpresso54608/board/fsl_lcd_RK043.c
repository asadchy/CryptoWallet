#include "fsl_sctimer.h"
#include "fsl_lcdc.h"
#include "fsl_lcd_RK043.h"

/* FRAMEBUFFER_ADDR */
#if (defined(__CC_ARM) || defined(__GNUC__))
__attribute__((aligned(16)))
#elif defined(__ICCARM__)
#pragma data_alignment = 16
#else
#error Toolchain not support.
#endif

uint16_t *framebuffer = (uint16_t *) SDRAM_BASE;

/*******************************************************************************
 * Code
 ******************************************************************************/

void LCD_Init_BL_PWM(void)
{
    sctimer_config_t config;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t event;

    CLOCK_AttachClk(kMCLK_to_SCT_CLK);

    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 2, true);

    SCTIMER_GetDefaultConfig(&config);

    SCTIMER_Init(SCT0, &config);

    pwmParam.output = kSCTIMER_Out_5;
    pwmParam.level = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = 5;

    SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 1000U, CLOCK_GetFreq(kCLOCK_Sct), &event);
}

status_t LCD_Init(void)
{
    /* Initialize the display. */
    lcdc_config_t lcdConfig;
    LCDC_GetDefaultConfig(&lcdConfig);

    lcdConfig.panelClock_Hz = LCD_PANEL_CLK;
    lcdConfig.ppl = LCD_PPL;
    lcdConfig.hsw = LCD_HSW;
    lcdConfig.hfp = LCD_HFP;
    lcdConfig.hbp = LCD_HBP;
    lcdConfig.lpp = LCD_LPP;
    lcdConfig.vsw = LCD_VSW;
    lcdConfig.vfp = LCD_VFP;
    lcdConfig.vbp = LCD_VBP;
    lcdConfig.polarityFlags = LCD_POL_FLAGS;
    lcdConfig.upperPanelAddr = (uint32_t)framebuffer;
    lcdConfig.bpp = kLCDC_16BPP565;
    lcdConfig.display = kLCDC_DisplayTFT;
    lcdConfig.swapRedBlue = true;
    lcdConfig.dataFormat = kLCDC_WinCeMode;

    LCDC_Init(APP_LCD, &lcdConfig, LCD_INPUT_CLK_FREQ);

    /* Trigger interrupt at start of every vertical back porch. */
//    LCDC_SetVerticalInterruptMode(APP_LCD, kLCDC_StartOfBackPorch);
//    LCDC_EnableInterrupts(APP_LCD, kLCDC_VerticalCompareInterrupt);
//    NVIC_EnableIRQ(APP_LCD_IRQn);

    LCDC_Start(APP_LCD);
    LCDC_PowerUp(APP_LCD);

    return kStatus_Success;
}
