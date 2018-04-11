#include <gui/status_screen/statusView.hpp>
#include <gui/status_screen/statusPresenter.hpp>

statusPresenter::statusPresenter(statusView& v)
    : view(v)
{
}

void statusPresenter::activate()
{

}

void statusPresenter::deactivate()
{

}

void statusPresenter::toMainScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoMainScreen();
}

void statusPresenter::toInitScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoInitScreen();
}

void statusPresenter::toStatusScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoStatusScreen();
}

void statusPresenter::toBlockedScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoBlockedScreen();
}

void statusPresenter::statusScreenEntered()
{
	model->statusScreenEntered();
}

void statusPresenter::setDialogText(touchgfx::Unicode::UnicodeChar *text)
{
	view.setDialogText(text);
}

void statusPresenter::walletStatus(struct wallet_status *status)
{
	view.walletStatus(status);
}

void statusPresenter::clearWallet()
{
	model->clearWallet();
}
