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

void startupPresenter::toPincodeScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoPincodeScreen();
}

void startupPresenter::toStatusScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoStatusScreen();
}
