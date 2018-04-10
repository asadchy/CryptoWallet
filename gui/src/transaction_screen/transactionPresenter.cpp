#include <gui/transaction_screen/transactionView.hpp>
#include <gui/transaction_screen/transactionPresenter.hpp>

transactionPresenter::transactionPresenter(transactionView& v)
    : view(v)
{
}

void transactionPresenter::activate()
{

}

void transactionPresenter::deactivate()
{

}

void transactionPresenter::walletTransaction(struct transaction *trans)
{
	view.walletTransaction(trans);
}

void transactionPresenter::transactionScreenEntered()
{
	model->transactionScreenEntered();
}

void transactionPresenter::setDialogText(touchgfx::Unicode::UnicodeChar *text)
{
	view.setDialogText(text);
}

void transactionPresenter::cancelPressed()
{
	model->cancelPressed();
}

void transactionPresenter::confirmPressed()
{
	model->confirmPressed();
}

void transactionPresenter::toStatusScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoStatusScreen();
}

void transactionPresenter::toPinScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoPinScreen();
}

void transactionPresenter::toInitScreen()
{
	static_cast<FrontendApplication*>(FrontendApplication::getInstance())->gotoInitScreen();
}


