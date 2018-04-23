#include <gui/startup_screen/startupView.hpp>
#include <gui/startup_screen/startupPresenter.hpp>

startupPresenter::startupPresenter(startupView& v)
    : view(v)
{
}

void startupPresenter::activate()
{

}

void startupPresenter::deactivate()
{

}

void startupPresenter::toStatusScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoStatusScreen();
}

void startupPresenter::toPinScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoPinScreen();
}

void startupPresenter::toBlockedScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoBlockedScreen();
}

void startupPresenter::toInitScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoInitScreen();
}

