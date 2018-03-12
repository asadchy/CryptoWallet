/*
 * dialog.hpp
 *
 *  Created on: Feb 23, 2018
 *      Author: artyom
 */

#ifndef DIALOG_HPP_
#define DIALOG_HPP_

#include <touchgfx/containers/ModalWindow.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <texts/TextKeysAndLanguages.hpp>

class Dialog : public touchgfx::ModalWindow
{
public:
	Dialog();

	void setText(touchgfx::Unicode::UnicodeChar *message);

private:
	touchgfx::Image backgroung;
	touchgfx::TextAreaWithOneWildcard text;
	touchgfx::ButtonWithLabel close;

	touchgfx::Unicode::UnicodeChar textBuffer[30];
	static const uint16_t TEXT_SIZE = 30;

	touchgfx::GenericCallback<> *callback;

	touchgfx::Callback<Dialog, const touchgfx::AbstractButton&> onButtonClicked;
	void buttonClicked(const touchgfx::AbstractButton& btn);
};

#endif /* DIALOG_HPP_ */
