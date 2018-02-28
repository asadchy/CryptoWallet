#ifndef STATUS_VIEW_HPP
#define STATUS_VIEW_HPP

#include <gui_generated/status_screen/statusViewBase.hpp>
#include <gui/status_screen/statusPresenter.hpp>
#include <gui/common/dialog.hpp>
#include <gui/common/ListElement.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/containers/ListLayout.hpp>

class statusView : public statusViewBase
{
public:
    statusView();
    virtual ~statusView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void statusScreenEntered();

    void setDialogText(touchgfx::Unicode::UnicodeChar *text);

private:
    static const int CURRENCY_NUM = 10;

    Dialog dialog;
    ScrollableContainer scroll;
    ListLayout list;
    ListElement listElements[CURRENCY_NUM];
};

#endif // STATUS_VIEW_HPP
