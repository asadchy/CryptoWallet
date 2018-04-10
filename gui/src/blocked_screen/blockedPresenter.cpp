#include <gui/blocked_screen/blockedView.hpp>
#include <gui/blocked_screen/blockedPresenter.hpp>

blockedPresenter::blockedPresenter(blockedView& v)
    : view(v)
{
}

void blockedPresenter::activate()
{

}

void blockedPresenter::deactivate()
{

}

void blockedPresenter::toBlockedScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoBlockedScreen();
}

void blockedPresenter::toInitScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoInitScreen();
}

void blockedPresenter::toPinScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoPinScreen();
}

void blockedPresenter::toStatusScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoStatusScreen();
}

void blockedPresenter::toMainScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoMainScreen();
}
