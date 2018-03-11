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

void transactionView::setCurrency(char *curr)
{
	uint16_t size;
	size = Unicode::strncpy(currencyBuffer, curr, CURRENCY_SIZE);
	if(size > 0)
		currencyBuffer[size] = 0;
	currency.invalidate();
}

void transactionView::setAddress(char *addr)
{
	uint16_t size;
	size = Unicode::strncpy(addressBuffer, addr, ADDRESS_SIZE);
	if(size > 0)
		addressBuffer[size] = 0;
	address.invalidate();
}

void transactionView::setValue(double val)
{
	Unicode::snprintfFloat(valueBuffer, VALUE_SIZE, "%1.10f", val);
	value.invalidate();
}

void transactionView::transactionScreenEntered()
{
	presenter->transactionScreenEntered();
}

void transactionView::setDialogText(touchgfx::Unicode::UnicodeChar *text)
{
	dialog.setText(text);
}
