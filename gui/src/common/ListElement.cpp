/*
 * ListElement.cpp
 *
 *  Created on: Feb 28, 2018
 *      Author: artyom
 */

#include <gui/common/ListElement.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>


ListElement::ListElement()
	: Container()
{
}

void ListElement::setListElement(const Bitmap &bmp)
{
	background.setBitmap(bmp);
	background.setXY(0,0);

	setHeight(background.getHeight());
	setWidth(background.getWidth());

	add(background);

	curr_name.setPosition(6, 5, 122, 20);
	curr_name.setColor(touchgfx::Color::getColorFrom24BitRGB(255,255,255));
	curr_name.setLinespacing(0);
	curr_name.setTypedText(TypedText(T_HEAD_TEXT));
	curr_nameBuffer[0] = 0;
	curr_name.setWildcard(curr_nameBuffer);
	add(curr_name);

	curr_amount.setPosition(4, 90, 122, 20);
	curr_amount.setColor(touchgfx::Color::getColorFrom24BitRGB(255,255,255));
	curr_amount.setLinespacing(0);
	curr_amount.setTypedText(TypedText(T_CURR_TEXT));
	curr_amountBuffer[0] = 0;
	curr_amount.setWildcard(curr_amountBuffer);
	add(curr_amount);

	curr_amount_dollars.setPosition(4, 150, 122, 20);
	curr_amount_dollars.setColor(touchgfx::Color::getColorFrom24BitRGB(255,255,255));
	curr_amount_dollars.setLinespacing(0);
	curr_amount_dollars.setTypedText(TypedText(T_CURR_TEXT));
	curr_amount_dollarsBuffer[0] = 0;
	curr_amount_dollars.setWildcard(curr_amount_dollarsBuffer);
	add(curr_amount_dollars);
}

void ListElement::setCurr(Unicode::UnicodeChar *name)
{
	Unicode::strncpy(curr_nameBuffer, name, BUF_SIZE);
	curr_name.invalidate();
}

void ListElement::setAmount(Unicode::UnicodeChar *amount)
{
	Unicode::strncpy(curr_amountBuffer, amount, BUF_SIZE);
	curr_amount.invalidate();
}

void ListElement::setAmountDollars(Unicode::UnicodeChar *amount)
{
	Unicode::strncpy(curr_amount_dollarsBuffer, amount, BUF_SIZE);
	curr_amount_dollars.invalidate();
}

void ListElement::setCurrIco(const Bitmap &bmp)
{
	ico.setBitmap(bmp);
	add(ico);
	ico.setXY(84, 44);
}
