/*
 * dialog.cpp
 *
 *  Created on: Feb 23, 2018
 *      Author: artyom
 */

#include <gui/common/dialog.hpp>
#include <touchgfx/Color.hpp>
#include <BitmapDatabase.hpp>

Dialog::Dialog() :
	callback(0),
	onButtonClicked(this, &Dialog::buttonClicked)
{
	setBackground(BITMAP_DIALOG_ID);
	setShadeColor(touchgfx::Color::getColorFrom24BitRGB(0x00, 0x00, 0x00));
	setShadeAlpha(127);

	text.setTypedText(touchgfx::TypedText(T_EMPTYTEXT));
	text.setWildcard(textBuffer);
	text.setPosition(10, 80, 300, 40);
	text.setColor(touchgfx::Color::getColorFrom24BitRGB(0x7E, 0xAE, 0xE5));
	ModalWindow::add(text);

	close.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_NOT_PRESSED_ID),
			touchgfx::Bitmap(BITMAP_BUTTON_PRESSED_ID));
	close.setLabelText(touchgfx::TypedText(T_CLOSE));
	close.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(0x7E, 0xAE, 0xE5));
	close.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(0x7E, 0xAE, 0xE5));
	close.setXY(112, 170);
	close.setAction(onButtonClicked);
	ModalWindow::add(close);
}

void Dialog::setText(touchgfx::Unicode::UnicodeChar *message)
{
	if(touchgfx::Unicode::strlen(message))
	{
		touchgfx::Unicode::snprintf(textBuffer, TEXT_SIZE, message);
		text.invalidate();
		show();
	}
	else
	{
		hide();
	}
}

void Dialog::buttonClicked(const touchgfx::AbstractButton& btn)
{
	hide();
}


