#include <gui/pin_screen/pinView.hpp>
#include <gui/pin_screen/pinPresenter.hpp>

pinPresenter::pinPresenter(pinView& v)
    : view(v)
{
}

void pinPresenter::activate()
{

}

void pinPresenter::deactivate()
{

}

void pinPresenter::setHeadText(touchgfx::Unicode::UnicodeChar *text)
{
	view.setHeadText(text);
}

void pinPresenter::pinScreenEntered()
{
	model->pinScreenEntered();
}

void pinPresenter::showPinKeyboard()
{
	view.showPinKeyboard();
}

void pinPresenter::showMSKeyboard()
{
	view.showMSKeyboard();
}

void pinPresenter::showMSWindow()
{
	view.showMSWindow();
}

void pinPresenter::pincodeEntered(int pincode)
{
	model->pincodeEntered(pincode);
}

void pinPresenter::msEntered(Unicode::UnicodeChar *mnemonic)
{
	model->msEntered(mnemonic);
}

void pinPresenter::cancelPressed()
{
	model->cancelPressed();
}

void pinPresenter::toInitScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoInitScreen();
}

void pinPresenter::toStatusScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoStatusScreen();
}

void pinPresenter::toPinScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoPinScreen();
}
