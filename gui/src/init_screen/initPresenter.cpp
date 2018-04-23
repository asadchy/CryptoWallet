#include <gui/init_screen/initView.hpp>
#include <gui/init_screen/initPresenter.hpp>

initPresenter::initPresenter(initView& v)
    : view(v)
{
}

void initPresenter::activate()
{

}

void initPresenter::deactivate()
{

}

void initPresenter::restoreWallet()
{
	model->restoreWallet();
}

void initPresenter::initWallet()
{
	model->initWallet();
}

void initPresenter::toPinScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoPinScreen();
}

void initPresenter::toBlockedScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoBlockedScreen();
}
