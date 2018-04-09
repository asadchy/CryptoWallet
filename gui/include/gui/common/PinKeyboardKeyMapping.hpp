/*
 * PinKeyboardKeyMapping.hpp
 *
 *  Created on: Mar 27, 2018
 *      Author: artyom
 */

#ifndef GUI_INCLUDE_GUI_COMMON_PINKEYBOARDKEYMAPPING_HPP_
#define GUI_INCLUDE_GUI_COMMON_PINKEYBOARDKEYMAPPING_HPP_

#include <touchgfx/widgets/Keyboard.hpp>
using namespace touchgfx;

/**
 * This file contains all the keymappings for the CustomKeyboard.
 */

static const Keyboard::KeyMapping pinKeyMappings[12] =
{
    { 1,  55},  // 7
    { 2,  56},  // 8
    { 3,  57},  // 9
    { 4,  52},  // 4
    { 5,  53},  // 5
    { 6,  54},  // 6
    { 7,  49},  // 1
    { 8,  50},  // 2
    { 9,  51},  // 3
    {11,  48}   // 0

};

static const Keyboard::KeyMappingList pinKeyMappingListNum =
{
	pinKeyMappings,
    12
};

#endif /* GUI_INCLUDE_GUI_COMMON_PINKEYBOARDKEYMAPPING_HPP_ */
