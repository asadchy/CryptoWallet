/*
 * PinKeyboardLayout.hpp
 *
 *  Created on: Mar 27, 2018
 *      Author: artyom
 */

#ifndef GUI_INCLUDE_GUI_COMMON_PINKEYBOARDLAYOUT_HPP_
#define GUI_INCLUDE_GUI_COMMON_PINKEYBOARDLAYOUT_HPP_

#include <touchgfx/widgets/Keyboard.hpp>
#include <touchgfx/hal/Types.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include "BitmapDatabase.hpp"

using namespace touchgfx;

#define NUM_BASE_X			30
#define NUM_BASE_Y			114

#define NUM_BUTN_SIZE_X		90
#define NUM_BUTN_SIZE_Y		32

#define NUM_BUTN_SPACE_X	(NUM_BUTN_SIZE_X + 15)
#define NUM_BUTN_SPACE_Y	(NUM_BUTN_SIZE_Y + 4)

#define NUM_KEY			BITMAP_NUM_BUTTON_PRESSED_ID

/**
 * Array specifying the keys used in the CustomKeyboard.
 */
static const Keyboard::Key pinKeyArray[12] =
{
    { 1, Rect(NUM_BASE_X, NUM_BASE_Y, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 2, Rect(NUM_BASE_X + NUM_BUTN_SPACE_X, NUM_BASE_Y, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 3, Rect(NUM_BASE_X + NUM_BUTN_SPACE_X * 2, NUM_BASE_Y, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 4, Rect(NUM_BASE_X, NUM_BASE_Y + NUM_BUTN_SPACE_Y, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 5, Rect(NUM_BASE_X + NUM_BUTN_SPACE_X, NUM_BASE_Y + NUM_BUTN_SPACE_Y, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 6, Rect(NUM_BASE_X + NUM_BUTN_SPACE_X * 2, NUM_BASE_Y + NUM_BUTN_SPACE_Y, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 7, Rect(NUM_BASE_X, NUM_BASE_Y + NUM_BUTN_SPACE_Y * 2, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 8, Rect(NUM_BASE_X + NUM_BUTN_SPACE_X, NUM_BASE_Y + NUM_BUTN_SPACE_Y * 2, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 9, Rect(NUM_BASE_X + NUM_BUTN_SPACE_X * 2, NUM_BASE_Y + NUM_BUTN_SPACE_Y * 2, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 10, Rect(NUM_BASE_X, NUM_BASE_Y + NUM_BUTN_SPACE_Y * 3, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 11, Rect(NUM_BASE_X + NUM_BUTN_SPACE_X, NUM_BASE_Y + NUM_BUTN_SPACE_Y * 3, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY},
    { 12, Rect(NUM_BASE_X + NUM_BUTN_SPACE_X * 2, NUM_BASE_Y + NUM_BUTN_SPACE_Y * 3, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), NUM_KEY}
};

/**
 * Callback areas for the special buttons on the CustomKeyboard.
 */
static Keyboard::CallbackArea pinCallbackAreas[3] =
{
    {Rect(NUM_BASE_X + NUM_BUTN_SPACE_X * 2, NUM_BASE_Y + NUM_BUTN_SPACE_Y * 3, NUM_BUTN_SIZE_X, NUM_BUTN_SIZE_Y), 0, NUM_KEY},  // backspace
	{Rect(364, 126, 96, 40), 0, BITMAP_BUTTON_PRESSED_ID},  	   // enter
	{Rect(364, 179, 96, 40), 0, BITMAP_BUTTON_PRESSED_ID},   //cancel
};

/**
 * The layout for the CustomKeyboard.
 */
static Keyboard::Layout pinLayout =
{
	BITMAP_PIN_KEYBOARD_ID,
    pinKeyArray,
    12,
    pinCallbackAreas,
    3,
    Rect(30, 38, 300, 40),
    TypedText(T_ENTEREDTEXT),
    0,
    Typography::BUTTON_TEXT,
    0
};

#endif /* GUI_INCLUDE_GUI_COMMON_PINKEYBOARDLAYOUT_HPP_ */
