#include <gui/status_screen/statusView.hpp>
#include <BitmapDatabase.hpp>
#include <string.h>

statusView::statusView() :
	buttonClickedCallback(this, &statusView::buttonClickedHandler),
	list(EAST),
	elementsCounter(0)
{

}

void statusView::setupScreen()
{
	dialog.setXY(HAL::DISPLAY_WIDTH / 2 - dialog.getWidth() / 2,
			HAL::DISPLAY_HEIGHT / 2 - dialog.getHeight() / 2);

	scroll.setPosition(25, 36, 430, 200);
	scroll.add(list);

	slideMenu.setup(SlideMenu::SOUTH, Bitmap(BITMAP_TOP_SLIDE_MENU_BACKGROUND_ID), Bitmap(BITMAP_TOP_SLIDE_MENU_BUTTON_ID),
			Bitmap(BITMAP_TOP_SLIDE_MENU_BUTTON_ID), 0, 0, (Bitmap(BITMAP_TOP_SLIDE_MENU_BACKGROUND_ID).getWidth() -
			Bitmap(BITMAP_TOP_SLIDE_MENU_BUTTON_ID).getWidth()) / 2,
			Bitmap(BITMAP_TOP_SLIDE_MENU_BACKGROUND_ID).getHeight() - 6);
	slideMenu.setXY(0, 0);
	slideMenu.setVisiblePixelsWhenCollapsed(Bitmap(BITMAP_TOP_SLIDE_MENU_BUTTON_ID).getHeight());
	slideMenu.setHiddenPixelsWhenExpanded(20);
	slideMenu.setAnimationEasingEquation(EasingEquations::backEaseInOut);
	slideMenu.setAnimationDuration(20); // Longer animation duration than default
	slideMenu.setExpandedStateTimeout(400); // Longer time out than default

	clearWalletButton.setBitmaps(Bitmap(BITMAP_CLEAR_WALLET_ID), Bitmap(BITMAP_CLEAR_WALLET_PRESSED_ID));
	clearWalletButton.setXY(2, slideMenu.getBackgroundY() + slideMenu.getHiddenPixelsWhenExpanded());
	clearWalletButton.setAction(buttonClickedCallback);
	slideMenu.add(clearWalletButton);

	add(scroll);
	add(slideMenu);
	add(dialog);

	statusScreenEntered();
}

void statusView::tearDownScreen()
{

}

void statusView::statusScreenEntered()
{
	presenter->statusScreenEntered();
}

void statusView::setDialogText(touchgfx::Unicode::UnicodeChar *text)
{
	dialog.setText(text);
}

void statusView::walletStatus(struct wallet_status *status)
{
	Unicode::UnicodeChar tmp[BUF_SIZE], tmp_ammount[BUF_SIZE];

	for(int i = 0; i < status->num; i++)
	{
		if(status->curr[i].curr_name < CURR_NUM)
		{
			if(elementsCounter > 0)
			{
				listElements[elementsCounter].setListElement(Bitmap(BITMAP_FILLER_ID));
				list.add(listElements[elementsCounter]);
				elementsCounter++;
			}
			listElements[elementsCounter].setListElement(Bitmap(BITMAP_CURR_STATUS_ID));
		}

		switch(status->curr[i].curr_name)
		{
			case BTC:
				listElements[elementsCounter].setCurrIco(Bitmap(BITMAP_BITCOIN_ID));
				Unicode::strncpy(tmp, currencies_list[status->curr[i].curr_name], BUF_SIZE);
				listElements[elementsCounter].setCurr(tmp);
				Unicode::strncpy(tmp_ammount, status->curr[i].amount, BUF_SIZE);
				Unicode::snprintf(tmp, BUF_SIZE, "%s BTC", tmp_ammount);
				listElements[elementsCounter].setAmount(tmp);
				Unicode::strncpy(tmp_ammount, status->curr[i].amount_dollars, BUF_SIZE);
				Unicode::snprintf(tmp, BUF_SIZE, "%s $", tmp_ammount);
				listElements[elementsCounter].setAmountDollars(tmp);
				break;

			case ETH:
				listElements[elementsCounter].setCurrIco(Bitmap(BITMAP_ETHER_ID));
				Unicode::strncpy(tmp, currencies_list[status->curr[i].curr_name], BUF_SIZE);
				listElements[elementsCounter].setCurr(tmp);
				Unicode::strncpy(tmp_ammount, status->curr[i].amount, BUF_SIZE);
				Unicode::snprintf(tmp, BUF_SIZE, "%s ETH", tmp_ammount);
				listElements[elementsCounter].setAmount(tmp);
				Unicode::strncpy(tmp_ammount, status->curr[i].amount_dollars, BUF_SIZE);
				Unicode::snprintf(tmp, BUF_SIZE, "%s $", tmp_ammount);
				listElements[elementsCounter].setAmountDollars(tmp);
				break;

			case LTC:
				listElements[elementsCounter].setCurrIco(Bitmap(BITMAP_LITECOIN_ID));
				Unicode::strncpy(tmp, currencies_list[status->curr[i].curr_name], BUF_SIZE);
				listElements[elementsCounter].setCurr(tmp);
				Unicode::strncpy(tmp_ammount, status->curr[i].amount, BUF_SIZE);
				Unicode::snprintf(tmp, BUF_SIZE, "%s LTC", tmp_ammount);
				listElements[elementsCounter].setAmount(tmp);
				Unicode::strncpy(tmp_ammount, status->curr[i].amount_dollars, BUF_SIZE);
				Unicode::snprintf(tmp, BUF_SIZE, "%s $", tmp_ammount);
				listElements[elementsCounter].setAmountDollars(tmp);
				break;

			default:
				break;
		}

		if(status->curr[i].curr_name < CURR_NUM)
		{
			list.add(listElements[elementsCounter]);
			elementsCounter++;
		}
	}
}

void statusView::buttonClickedHandler(const AbstractButton& button)
{
	if(&button == &clearWalletButton)
	{
		clearWallet();
	}
}

void statusView::clearWallet()
{
	presenter->clearWallet();
}

