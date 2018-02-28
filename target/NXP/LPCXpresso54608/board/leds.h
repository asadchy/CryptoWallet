#ifndef __LEDS_H_
#define __LEDS_H_
#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

void Board_LED_Init();

/**
 * @brief	Sets the state of a board LED to on or off
 * @param	LEDNumber	: LED number to set state for
 * @param	State		: true for on, false for off
 * @return	None
 */
void Board_LED_Set(uint8_t LEDNumber, bool State);

/**
 * @brief	Returns the current state of a board LED
 * @param	LEDNumber	: LED number to set state for
 * @return	true if the LED is on, otherwise false
 */
bool Board_LED_Test(uint8_t LEDNumber);

/**
 * @brief	Toggles the current state of a board LED
 * @param	LEDNumber	: LED number to change state for
 * @return	None
 */
void Board_LED_Toggle(uint8_t LEDNumber);

#ifdef __cplusplus
}
#endif

#endif /* __LEDS_H_ */
