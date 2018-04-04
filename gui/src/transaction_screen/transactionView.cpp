#include <gui/transaction_screen/transactionView.hpp>
#include <stdio.h>

transactionView::transactionView() :
	onButtonClicked(this, &transactionView::buttonClicked)
{

}

void transactionView::setupScreen()
{
	add(dialog);
	dialog.setXY(HAL::DISPLAY_WIDTH / 2 - dialog.getWidth() / 2,
			HAL::DISPLAY_HEIGHT / 2 - dialog.getHeight() / 2);

	cancel.setAction(onButtonClicked);
	confirm.setAction(onButtonClicked);

	transactionScreenEntered();
}

void transactionView::tearDownScreen()
{

}

void transactionView::buttonClicked(const touchgfx::AbstractButton& btn)
{
	if(&btn == &cancel)
	{
		presenter->cancelPressed();
	}
	else if(&btn == &confirm)
	{
		presenter->confirmPressed();
	}
}

void transactionView::walletTransaction(struct transaction *trans)
{
	uint16_t size;
	size = Unicode::strncpy(currencyBuffer, currencies_list[trans->curr_name], CURRENCY_SIZE);
	if(size > 0)
		currencyBuffer[size] = 0;
	currency.invalidate();

	size = Unicode::strncpy(addressBuffer, trans->addr, ADDRESS_SIZE);
	if(size > 0)
		addressBuffer[size] = 0;
	address.invalidate();

	size = Unicode::strncpy(currencyBuffer, trans->value, CURRENCY_SIZE);
	if(size > 0)
		currencyBuffer[size] = 0;
	currency.invalidate();
}

void transactionView::transactionScreenEntered()
{
	presenter->transactionScreenEntered();
}

void transactionView::setDialogText(touchgfx::Unicode::UnicodeChar *text)
{
	dialog.setText(text);
}
