#ifndef STATUS_VIEW_HPP
#define STATUS_VIEW_HPP

#include <gui_generated/status_screen/statusViewBase.hpp>
#include <gui/status_screen/statusPresenter.hpp>
#include <gui/common/dialog.hpp>
#include <gui/common/ListElement.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <data.hpp>
#include <touchgfx/containers/SlideMenu.hpp>
#include <touchgfx/widgets/Button.hpp>

class statusView : public statusViewBase
{
public:
    statusView();
    virtual ~statusView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void statusScreenEntered();

    void setDialogText(touchgfx::Unicode::UnicodeChar *text);

    void walletStatus(struct wallet_status *status);

    void clearWallet();

private:
    static const int CURRENCY_NUM = 10;
    static const uint16_t BUF_SIZE = 16;

    SlideMenu slideMenu;
    Button clearWalletButton;
    Callback<statusView, const AbstractButton&> buttonClickedCallback;
    Dialog dialog;
    ScrollableContainer scroll;
    ListLayout list;
    ListElement listElements[2 * CURRENCY_NUM - 1];
    int elementsCounter;

    void buttonClickedHandler(const AbstractButton& button);
};

#endif // STATUS_VIEW_HPP
