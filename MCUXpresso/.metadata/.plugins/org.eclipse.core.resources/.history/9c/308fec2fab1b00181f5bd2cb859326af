#include <gui/pin_code_screen/pin_codeView.hpp>

pin_codeView::pin_codeView() :
	pincodeEnteredCallback(this, &pin_codeView::pincodeEntered)
{
}

void pin_codeView::setupScreen()
{
	keyboard.setPosition(0, 0, 480, 272);
	keyboard.setPincodeEnteredCallback(pincodeEnteredCallback);
	add(keyboard);

	add(dialog);
	dialog.setXY(HAL::DISPLAY_WIDTH / 2 - dialog.getWidth() / 2,
			HAL::DISPLAY_HEIGHT / 2 - dialog.getHeight() / 2);

	pincodeScreenEntered();
}

void pin_codeView::tearDownScreen()
{

}

void pin_codeView::pincodeEntered(int pincode)
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

void pin_codeView::pincodeScreenEntered()
{
	presenter->pincodeScreenEntered();
}

void pin_codeView::setDialogText(touchgfx::Unicode::UnicodeChar *text)
{
	dialog.setText(text);
}
