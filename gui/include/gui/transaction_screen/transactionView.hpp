#ifndef TRANSACTION_VIEW_HPP
#define TRANSACTION_VIEW_HPP

#include <gui_generated/transaction_screen/transactionViewBase.hpp>
#include <gui/transaction_screen/transactionPresenter.hpp>
#include <gui/common/dialog.hpp>

class transactionView : public transactionViewBase
{
public:
    transactionView();
    virtual ~transactionView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void walletTransaction(struct transaction *trans);

	void transactionScreenEntered();
	void setDialogText(touchgfx::Unicode::UnicodeChar *text);
private:
	Dialog dialog;

	touchgfx::Callback<transactionView, const touchgfx::AbstractButton&> onButtonClicked;
	void buttonClicked(const touchgfx::AbstractButton& btn);
};

#endif // TRANSACTION_VIEW_HPP
