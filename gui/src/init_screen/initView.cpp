#include <gui/init_screen/initView.hpp>

initView::initView() :
	buttonPressed(this, &initView::buttonPressedHandler)
{

}

void initView::setupScreen()
{
	restore_button.setAction(buttonPressed);
	generate_button.setAction(buttonPressed);
}

void initView::tearDownScreen()
{

}

void initView::buttonPressedHandler(const touchgfx::AbstractButton& btn)
{
	if(&btn == &restore_button)
	{
		presenter->restoreWallet();
	}
	else if(&btn == &generate_button)
	{
		presenter->initWallet();
	}
}
