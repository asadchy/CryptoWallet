#include "leds.h"
#include "board.h"

typedef struct {
	uint8_t port;
	uint8_t pin;
} PORT_PIN_T;
//								3	4	5	6	7
//					0	1       2     J13-19  J13-7   J13-9  J13-11 J13-13
static const PORT_PIN_T ledBits[] = {{3, 14}, {3, 3}, {2, 2}, {4, 4}, {3,10}, {4,5}, {3,2}, {3,27}};
static const uint32_t ledBits_ct = sizeof(ledBits) / sizeof(PORT_PIN_T);

/* Initialize the LEDs on the NXP LPC5460x NXP Eval Board */
void Board_LED_Init(void)
{
	uint32_t i;

	/* Pin muxing setup as part of board_sysinit */
	for (i = 0; i < ledBits_ct; i++) 
        {
          GPIO_PinInit(GPIO,ledBits[i].port, ledBits[i].pin,&(gpio_pin_config_t){kGPIO_DigitalOutput, (LOGIC_LED_OFF)});
	}
}

/* Set the LED to the state of "On" */
void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber < ledBits_ct) 
        {
          if (On)
          {
            GPIO_ClearPinsOutput(GPIO, ledBits[LEDNumber].port, 1U << ledBits[LEDNumber].pin); /*!< Turn on target LED */
          }
          else
          {
             GPIO_SetPinsOutput(GPIO, ledBits[LEDNumber].port, 1U << ledBits[LEDNumber].pin);/*!< Turn off target LED */
          }
	}
}

/* Toggles the current state of a board LED */
void Board_LED_Toggle(uint8_t LEDNumber)
{
	if (LEDNumber < ledBits_ct) 
        {
                  GPIO_TogglePinsOutput(GPIO, ledBits[LEDNumber].port, 1U << ledBits[LEDNumber].pin);
	}
}
