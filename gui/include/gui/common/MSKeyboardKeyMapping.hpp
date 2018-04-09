/*
 * MSKeyboardKeyMapping.hpp
 *
 *  Created on: Mar 28, 2018
 *      Author: artyom
 */

#ifndef GUI_INCLUDE_GUI_COMMON_MSKEYBOARDKEYMAPPING_HPP_
#define GUI_INCLUDE_GUI_COMMON_MSKEYBOARDKEYMAPPING_HPP_

#include <touchgfx/widgets/Keyboard.hpp>
using namespace touchgfx;

/**
 * This file contains all the keymappings for the CustomKeyboard.
 */

static const Keyboard::KeyMapping MSKeyMappings[27] =
{
		{ 1, 113},  // q
		{ 2, 119},  // w
		{ 3, 101},  // e
		{ 4, 114},  // r
		{ 5, 116},  // t
		{ 6, 121},  // y
		{ 7, 117},  // u
		{ 8, 105},  // i
		{ 9, 111},  // o
		{10, 112},  // p
		{11,  97},  // a
		{12, 115},  // s
		{13, 100},  // d
		{14, 102},  // f
		{15, 103},  // g
		{16, 104},  // h
		{17, 106},  // j
		{18, 107},  // k
		{19, 108},  // l
		{20, 122},  // z
		{21, 120},  // x
		{22,  99},  // c
		{23, 118},  // v
		{24,  98},  // b
		{25, 110},  // n
		{26, 109},  // m
		{27,  32},  // space
};

static const Keyboard::KeyMappingList MSKeyMappingListNum =
{
	MSKeyMappings,
    27
};

#endif /* GUI_INCLUDE_GUI_COMMON_MSKEYBOARDKEYMAPPING_HPP_ */
