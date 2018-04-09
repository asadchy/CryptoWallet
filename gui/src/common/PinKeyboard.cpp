/*
 * PinKeyboard.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: artyom
 */

#include <gui/common/PinKeyboard.hpp>

PinKeyboard::PinKeyboard() : keyboard(),
	enterPressed(this, &PinKeyboard::enterPressedHandler),
    backspacePressed(this, &PinKeyboard::backspacePressedHandler),
	cancelPressed(this, &PinKeyboard::cancelPressedHandler),
    keyPressed(this, &PinKeyboard::keyPressedhandler),
    alphaKeys(false),
    uppercaseKeys(false),
    firstCharacterEntry(false)
{
    //Set the callbacks for the callback areas of the keyboard and set its layout.
    pinLayout.callbackAreaArray[0].callback = &backspacePressed;
    pinLayout.callbackAreaArray[1].callback = &enterPressed;
    pinLayout.callbackAreaArray[2].callback = &cancelPressed;
    pinLayout.keyFontColor = touchgfx::Color::getColorFrom24BitRGB(0x7E, 0xAE, 0xE5);
    pinLayout.textAreaFontColor = touchgfx::Color::getColorFrom24BitRGB(0x7E, 0xAE, 0xE5);
    keyboard.setLayout(&pinLayout);
    keyboard.setKeyListener(keyPressed);
    keyboard.setPosition(0, 0, 480, 272);
    keyboard.setTextIndentation();
    //Allocate the buffer associated with keyboard.
    memset(buffer, 0, sizeof(buffer));
    keyboard.setBuffer(buffer, BUFFER_SIZE);

    uppercaseKeys = true;
    firstCharacterEntry = true;

    enterTextArea.setPosition(364, 133, 96, 20);
	enterTextArea.setTypedText(TypedText(T_ENTER));
	enterTextArea.setColor(touchgfx::Color::getColorFrom24BitRGB(0x7E, 0xAE, 0xE5));

	cancelTextArea.setPosition(364, 186, 96, 20);
	cancelTextArea.setTypedText(TypedText(T_CANCEL_PIN));
	cancelTextArea.setColor(touchgfx::Color::getColorFrom24BitRGB(0x7E, 0xAE, 0xE5));

	setKeyMappingList();

	add(keyboard);
	add(enterTextArea);
	add(cancelTextArea);
}

void PinKeyboard::setKeyMappingList()
{
	keyboard.setKeymappingList(&pinKeyMappingListNum);
}

void PinKeyboard::backspacePressedHandler()
{
    uint16_t pos = keyboard.getBufferPosition();
    if (pos > 0)
    {
        //Delete the previous entry in the buffer and decrement the position.
        buffer[pos - 1] = 0;
        shadow_buffer[pos - 1] = 0;
        keyboard.setBufferPosition(pos - 1);
    }
}

void PinKeyboard::enterPressedHandler()
{
	uint16_t pos = keyboard.getBufferPosition();
	if(pos == BUFFER_SIZE - 1)
	{
		int i;
		if(pincodeEntered->isValid())
		{
			pincodeEntered->execute(Unicode::atoi(shadow_buffer));
		}
		for(i = 0; i < BUFFER_SIZE; i++)
		{
			buffer[i] = shadow_buffer[i] = 0;
		}
		keyboard.setBufferPosition(0);
	}
	else
	{
		if(pincodeEntered->isValid())
		{
			pincodeEntered->execute(~1);
		}
	}
}

void PinKeyboard::cancelPressedHandler()
{
	int i;
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = shadow_buffer[i] = 0;
	}
	keyboard.setBufferPosition(0);

	if(opCancel->isValid())
	{
		opCancel->execute();
	}
}

void PinKeyboard::keyPressedhandler(Unicode::UnicodeChar keyChar)
{
	if(keyChar > 0)
	{
		uint16_t pos = keyboard.getBufferPosition();
		if(pos > 0)
		{
			shadow_buffer[pos - 1] = buffer[pos - 1];
			buffer[pos - 1] = '*';
		}
	}
}

void PinKeyboard::setTouchable(bool touch)
{
    Container::setTouchable(touch);
    keyboard.setTouchable(touch);
}

void PinKeyboard::setPincodeEnteredCallback(Callback<pinView, int> &callback)
{
	pincodeEntered = &callback;
}

void PinKeyboard::setOpCancelCallback(Callback<pinView, void> &callback)
{
	opCancel = &callback;
}
