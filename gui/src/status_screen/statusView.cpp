#include <gui/status_screen/statusView.hpp>
#include <BitmapDatabase.hpp>

statusView::statusView()
{

}

void statusView::setupScreen()
{
	dialog.setXY(HAL::DISPLAY_WIDTH / 2 - dialog.getWidth() / 2,
			HAL::DISPLAY_HEIGHT / 2 - dialog.getHeight() / 2);

	listElements[0].setListElement(Bitmap(BITMAP_LIST_ELEMENT_BTC_ID));
	listElements[1].setListElement(Bitmap(BITMAP_LIST_ELEMENT_ETH_ID));
	listElements[2].setListElement(Bitmap(BITMAP_LIST_ELEMENT_LIT_ID));
	listElements[3].setListElement(Bitmap(BITMAP_LIST_ELEMENT_BTC_ID));
	listElements[4].setListElement(Bitmap(BITMAP_LIST_ELEMENT_ETH_ID));
	listElements[5].setListElement(Bitmap(BITMAP_LIST_ELEMENT_LIT_ID));
	listElements[6].setListElement(Bitmap(BITMAP_LIST_ELEMENT_BTC_ID));
	listElements[7].setListElement(Bitmap(BITMAP_LIST_ELEMENT_ETH_ID));
	listElements[8].setListElement(Bitmap(BITMAP_LIST_ELEMENT_LIT_ID));
	listElements[9].setListElement(Bitmap(BITMAP_LIST_ELEMENT_LIT_ID));

	for(int i = 0; i < CURRENCY_NUM; i++)
	{
		list.add(listElements[i]);
	}

	scroll.setPosition(194, 65, 262, 162);
	scroll.add(list);

	add(scroll);
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
