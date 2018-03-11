#include <gui/status_screen/statusView.hpp>
#include <BitmapDatabase.hpp>

statusView::statusView()
{

}

void statusView::setupScreen()
{
	dialog.setXY(HAL::DISPLAY_WIDTH / 2 - dialog.getWidth() / 2,
			HAL::DISPLAY_HEIGHT / 2 - dialog.getHeight() / 2);

	listElements[0].setListElement(Bitmap(BITMAP_BTC_STATUS_ID));
	listElements[1].setListElement(Bitmap(BITMAP_ETH_STATUS_ID));
	listElements[2].setListElement(Bitmap(BITMAP_LITE_STATUS_ID));

	for(int i = 0; i < CURRENCY_NUM; i++)
	{
		list.add(listElements[i]);
	}

	scroll.setPosition(222, 65, 218, 124);
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
