/*
 * MSKeyboard.hpp
 *
 *  Created on: Mar 28, 2018
 *      Author: artyom
 */

#ifndef GUI_INCLUDE_GUI_COMMON_MSKEYBOARD_HPP_
#define GUI_INCLUDE_GUI_COMMON_MSKEYBOARD_HPP_

#include <touchgfx/widgets/Keyboard.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/common/MSKeyboardLayout.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include <gui/common/MSKeyboardKeyMapping.hpp>

class pinView;

using namespace touchgfx;

/**
 * An alphanumeric keyboard with backspace, space-bar and delete functionality.
 * Keyboard mappings for the keyboard are defined in gui/include/gui/common/KeyboardKeyMapping.hpp.
 * Keyboard layout for the keyboard is defined in gui/include/gui/common/KeyboardLayout.hpp.
 * Please note that the ApplicationFontProvider is initialized and set with the FontManager in main.cpp
 * The fonts and the characters used by the keyboard have to be defined in the assets/texts/texts.xlsx file.
 */
class MSKeyboard : public Container
{
public:
	MSKeyboard();
    virtual ~MSKeyboard() { }

    /*
     * Override setTouchable to also affect the keyboard object inside this
     * CustomKeyboard.
     */
    void setTouchable(bool touch);

    void setMSEnteredCallback(Callback<pinView, Unicode::UnicodeChar*> &callback);

    void setOpCancelCallback(Callback<pinView, void> &callback);

private:
    /*
     * The size of the buffer that is used by the keyboard.
     * The size determines how much text the keyboard can contain in its textfield.
     */
    static const uint8_t BUFFER_SIZE = 123;

    /**
     * The buffer used by the keyboard for text input.
     */
    Unicode::UnicodeChar buffer[BUFFER_SIZE];

    /**
	 * The keyboard which this CustomKeyboard wraps.
	 */
	Keyboard keyboard;

    /**
     * Callback for the capslock button.
     */
    Callback<MSKeyboard> enterPressed;

    /**
     * Callback for the backspace button.
     */
    Callback<MSKeyboard> backspacePressed;

    /**
	 * Callback for the cancel button.
	 */
	Callback<MSKeyboard> cancelPressed;

    /**
     * Callback for when keys are pressed on the keyboard.
     */
    Callback<MSKeyboard, Unicode::UnicodeChar> keyPressed;

    /**
	 * Callback for when pin code entered.
	 */
    Callback<pinView, Unicode::UnicodeChar*> *MSEntered;

    Callback<pinView, void> *opCancel;

    /**
	 * Used to display text on top of the button Enter.
	 */
	TextArea enterTextArea;

	/**
	 * Used to display text on top of the button Cancel.
	 */
	TextArea cancelTextArea;

    /**
     * True if the keyboard should show alpha keys, false for numeric.
     */
    bool alphaKeys;

    /**
     * True if the keyboard should show upper-case keys.
     */
    bool uppercaseKeys;

    /**
     * True if the input position in the text field, and hence the buffer, is at the beginning.
     */
    bool firstCharacterEntry;

    /*
     * Sets the correct key mappings of the keyboard according to alpha/numeric and upper-case/lower-case.
     */
    void setKeyMappingList();

    /**
     * Callback handler for the backspace button.
     */
    void backspacePressedHandler();

	/**
	 * Callback handler for the enter button.
	 */
	void enterPressedHandler();

	/**
	 * Callback handler for the cancel button.
	 */
	void cancelPressedHandler();


    /**
     * Callback handler for key presses.
     * @param keyChar The UnicodeChar for the key that was pressed.
     */
    void keyPressedhandler(Unicode::UnicodeChar keyChar);

    int wordCounter;
    static const uint8_t WORDS_NUM = 12;
};

#endif /* GUI_INCLUDE_GUI_COMMON_MSKEYBOARD_HPP_ */
