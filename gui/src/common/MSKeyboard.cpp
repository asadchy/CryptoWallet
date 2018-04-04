/*
 * MSKeyboard.cpp
 *
 *  Created on: Mar 28, 2018
 *      Author: artyom
 */

#include <gui/common/MSKeyboard.hpp>

MSKeyboard::MSKeyboard() : keyboard(),
	enterPressed(this, &MSKeyboard::enterPressedHandler),
    backspacePressed(this, &MSKeyboard::backspacePressedHandler),
	cancelPressed(this, &MSKeyboard::cancelPressedHandler),
    keyPressed(this, &MSKeyboard::keyPressedhandler),
    alphaKeys(false),
    uppercaseKeys(false),
    firstCharacterEntry(false)
{
    //Set the callbacks for the callback areas of the keyboard and set its layout.
    MSLayout.callbackAreaArray[0].callback = &backspacePressed;
    MSLayout.callbackAreaArray[1].callback = &enterPressed;
    MSLayout.callbackAreaArray[2].callback = &cancelPressed;
    MSLayout.keyFontColor = touchgfx::Color::getColorFrom24BitRGB(0x7E, 0xAE, 0xE5);
    MSLayout.textAreaFontColor = touchgfx::Color::getColorFrom24BitRGB(0x7E, 0xAE, 0xE5);
    keyboard.setLayout(&MSLayout);
    keyboard.setKeyListener(keyPressed);
    keyboard.setPosition(0, 0, 480, 272);
    keyboard.setTextIndentation();
    //Allocate the buffer associated with keyboard.
    memset(buffer, 0, sizeof(buffer));
    keyboard.setBuffer(buffer, BUFFER_SIZE);

    uppercaseKeys = true;
    firstCharacterEntry = true;

    wordCounter = 0;

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

void MSKeyboard::setKeyMappingList()
{
	keyboard.setKeymappingList(&MSKeyMappingListNum);
}

void MSKeyboard::backspacePressedHandler()
{
    uint16_t pos = keyboard.getBufferPosition();
    if (pos > 0)
    {
        //Delete the previous entry in the buffer and decrement the position.
    	if(buffer[pos - 1] == '\n')
    	{
    		buffer[pos - 1] = 0;
			keyboard.setBufferPosition(pos - 1);
			pos -= 1;
    	}
    	if(buffer[pos - 1] == ' ')
    	{
    		--wordCounter;
    	}
        buffer[pos - 1] = 0;
        keyboard.setBufferPosition(pos - 1);
    }
}

void MSKeyboard::enterPressedHandler()
{
	if(MSEntered->isValid())
	{
		if(wordCounter == WORDS_NUM - 1)
		{
			MSEntered->execute(buffer);
		}
		else
		{
			MSEntered->execute(NULL);
		}
	}
}

void MSKeyboard::cancelPressedHandler()
{
	int i;
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = 0;
	}
	keyboard.setBufferPosition(0);
	wordCounter = 0;

	if(opCancel->isValid())
	{
		opCancel->execute();
	}
}

void MSKeyboard::keyPressedhandler(Unicode::UnicodeChar keyChar)
{
	//TODO handle long words (for example 40 symbols)
	uint16_t pos = keyboard.getBufferPosition();
	if(keyChar > 0)
	{
		if(pos == 40 || pos == 80)
		{
			if(buffer[pos - 1] != ' ')
			{
				int i;
				for(i = pos; i > 0 && buffer[i - 1] != ' '; i--)
				{
					buffer[i] = buffer[i - 1];
				}
				buffer[i] = '\n';
				keyboard.setBufferPosition(++pos);
			}
			else
			{
				buffer[pos] = '\n';
				keyboard.setBufferPosition(++pos);
			}
		}
		if(keyChar == ' ')
		{
			++wordCounter;
		}
	}
}

void MSKeyboard::setTouchable(bool touch)
{
    Container::setTouchable(touch);
    keyboard.setTouchable(touch);
}

void MSKeyboard::setMSEnteredCallback(Callback<pinView, Unicode::UnicodeChar*> &callback)
{
	MSEntered = &callback;
}

void MSKeyboard::setOpCancelCallback(Callback<pinView, void> &callback)
{
	opCancel = &callback;
}


