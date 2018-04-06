#include <gui/pin_screen/pinView.hpp>
#include <stdlib.h>
#include <string.h>
#include <wordlist.hpp>

pinView::pinView() :
	pincodeEnteredCallback(this, &pinView::pincodeEntered),
	MSEnteredCallback(this, &pinView::MSEntered),
	cancelPressedCallback(this, &pinView::cancelPressed),
	closePressedCallback(this, &pinView::closePressedHandler)
{
}

void pinView::setupScreen()
{
	pinKeyboard.setPosition(0, 0, 480, 272);
	pinKeyboard.setPincodeEnteredCallback(pincodeEnteredCallback);
	pinKeyboard.setOpCancelCallback(cancelPressedCallback);
	add(pinKeyboard);
	pinKeyboard.setVisible(false);

	msKeyboard.setPosition(0, 0, 480, 272);
	msKeyboard.setMSEnteredCallback(MSEnteredCallback);
	msKeyboard.setOpCancelCallback(cancelPressedCallback);
	add(msKeyboard);
	msKeyboard.setVisible(false);

	add(dialog);
	dialog.setXY(HAL::DISPLAY_WIDTH / 2 - dialog.getWidth() / 2,
			HAL::DISPLAY_HEIGHT / 2 - dialog.getHeight() / 2);
	dialog.hide();

	msWindow.setPosition(0, 0, 480, 272);
	msWindow.setBitmap(Bitmap(BITMAP_MS_WINDOW_ID));
	closeBtn.setPosition(364, 126, 96, 40);
	closeBtn.setBitmaps(Bitmap(BITMAP_BUTTON_NOT_PRESSED_ID), Bitmap(BITMAP_BUTTON_PRESSED_ID));
	closeBtn.setLabelText(TypedText(T_CLOSE));
	closeBtn.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(126,174,229));
	closeBtn.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(126,174,229));
	closeBtn.setAction(closePressedCallback);
	mnemonicSeed.setPosition(36, 50, 302, 60);
	mnemonicSeed.setColor(touchgfx::Color::getColorFrom24BitRGB(126,174,229));
	mnemonicSeed.setLinespacing(0);
	mnemonicSeed.setTypedText(TypedText(T_MSW_TEXT));
	mnemonicSeedBuffer[0] = '1';
	mnemonicSeedBuffer[1] = 0;
	mnemonicSeed.setWildcard(mnemonicSeedBuffer);
	add(closeBtn);
	add(msWindow);
	add(mnemonicSeed);
	mnemonicSeed.setVisible(false);
	closeBtn.setVisible(false);
	msWindow.setVisible(false);

	pinScreenEntered();
}

void pinView::tearDownScreen()
{

}

void pinView::showPinKeyboard()
{
	mnemonicSeed.setVisible(false);
	closeBtn.setVisible(false);
	msWindow.setVisible(false);
	msKeyboard.setVisible(false);
	pinKeyboard.setVisible(true);
	pinKeyboard.invalidate();
}

void pinView::showMSKeyboard()
{
	mnemonicSeed.setVisible(false);
	closeBtn.setVisible(false);
	msWindow.setVisible(false);
	pinKeyboard.setVisible(false);
	msKeyboard.setVisible(true);
	msKeyboard.invalidate();
}

void pinView::showMSWindow()
{
	pinKeyboard.setVisible(false);
	msKeyboard.setVisible(false);
	mnemonicSeed.setVisible(true);
	closeBtn.setVisible(true);
	msWindow.setVisible(true);
	mnemonicSeed.invalidate();
	closeBtn.invalidate();
	msKeyboard.invalidate();
}

void pinView::setHeadText(touchgfx::Unicode::UnicodeChar *text)
{
	touchgfx::Unicode::snprintf(head_messBuffer, HEAD_MESS_SIZE, text);
	head_mess.invalidate();
}

void pinView::pincodeEntered(int pincode)
{
	if(pincode == ~1)
	{
		static touchgfx::Unicode::UnicodeChar err[30];
		touchgfx::Unicode::strncpy(err, "PIN must contain 4 digits", 30);
		dialog.setText(err);
	}
	else
	{
		presenter->pincodeEntered(pincode);
	}
}

void pinView::MSEntered(Unicode::UnicodeChar *mnemonic)
{
	if(mnemonic == NULL)
	{
		static touchgfx::Unicode::UnicodeChar err[30];
		touchgfx::Unicode::strncpy(err, "MS must contain 12 words", 30);
		dialog.setText(err);
	}
	else
	{
		presenter->msEntered(mnemonic);
	}
}

void pinView::cancelPressed()
{
	presenter->cancelPressed();
}

void pinView::closePressed()
{
	presenter->closePressed();
}

void pinView::closePressedHandler(const touchgfx::AbstractButton& btn)
{
	closePressed();
}

void pinView::pinScreenEntered()
{
	presenter->pinScreenEntered();
}

void pinView::setMnemonicSeed(Unicode::UnicodeChar *mnemonic)
{
	memset(mnemonicSeedBuffer, 0, MNEMONICSEED_SIZE);
	Unicode::strncpy(mnemonicSeedBuffer, mnemonic, MNEMONICSEED_SIZE);
	memset(mnemonic, 0, MNEMONICSEED_SIZE);
	mnemonicSeed.invalidate();
}
