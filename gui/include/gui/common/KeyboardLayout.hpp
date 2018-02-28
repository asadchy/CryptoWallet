/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.8.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * This file is free software and is provided for example purposes. You may
 * use, copy, and modify within the terms and conditions of the license
 * agreement.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#ifndef KEYBOARD_LAYOUT
#define KEYBOARD_LAYOUT

#include <touchgfx/widgets/Keyboard.hpp>
#include <touchgfx/hal/Types.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include "BitmapDatabase.hpp"

using namespace touchgfx;

#define BASE_X			180
#define BASE_Y			68

#define BUTN_SIZE_X		39
#define BUTN_SIZE_Y		45

#define BUTN_SPACE_X	(BUTN_SIZE_X + 3)
#define BUTN_SPACE_Y	(BUTN_SIZE_Y + 5)

/**
 * Array specifying the keys used in the CustomKeyboard.
 */
static const Keyboard::Key keyArray[12] =
{
    { 1, Rect(BASE_X, BASE_Y, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 2, Rect(BASE_X + BUTN_SPACE_X, BASE_Y, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 3, Rect(BASE_X + BUTN_SPACE_X * 2, BASE_Y, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 4, Rect(BASE_X, BASE_Y + BUTN_SPACE_Y, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 5, Rect(BASE_X + BUTN_SPACE_X, BASE_Y + BUTN_SPACE_Y, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 6, Rect(BASE_X + BUTN_SPACE_X * 2, BASE_Y + BUTN_SPACE_Y, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 7, Rect(BASE_X, BASE_Y + BUTN_SPACE_Y * 2, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 8, Rect(BASE_X + BUTN_SPACE_X, BASE_Y + BUTN_SPACE_Y * 2, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 9, Rect(BASE_X + BUTN_SPACE_X * 2, BASE_Y + BUTN_SPACE_Y * 2, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 10, Rect(BASE_X, BASE_Y + BUTN_SPACE_Y * 3, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 11, Rect(BASE_X + BUTN_SPACE_X, BASE_Y + BUTN_SPACE_Y * 3, BUTN_SIZE_X, BUTN_SIZE_Y), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}
};

/**
 * Callback areas for the special buttons on the CustomKeyboard.
 */
static Keyboard::CallbackArea callbackAreas[2] =
{
    {Rect(BASE_X + BUTN_SPACE_X * 2, BASE_Y + BUTN_SPACE_Y * 3, BUTN_SIZE_X, BUTN_SIZE_Y), 0, BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},  // backspace
	{Rect(BASE_X, BASE_Y + BUTN_SPACE_Y * 3, BUTN_SIZE_X, BUTN_SIZE_Y), 0, BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},  // enter
};

/**
 * The layout for the CustomKeyboard.
 */
static const Keyboard::Layout layout =
{
    BITMAP_KEYBOARD_BACKGROUND_ID,
    keyArray,
    12,
    callbackAreas,
    2,
    Rect(18, 14, 450, 54),
    TypedText(T_ENTEREDTEXT),
#if !defined(USE_BPP) || USE_BPP==16
    0xFFFF,
#elif USE_BPP==24
    0xFFFFFF,
#elif USE_BPP==4
    0xF,
#elif USE_BPP==2
    0x3,
#else
#error Unknown USE_BPP
#endif
    Typography::KEYBOARD,
    0
};

#endif
