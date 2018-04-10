#ifndef BLOCKED_VIEW_HPP
#define BLOCKED_VIEW_HPP

#include <gui_generated/blocked_screen/blockedViewBase.hpp>
#include <gui/blocked_screen/blockedPresenter.hpp>

class blockedView : public blockedViewBase
{
public:
    blockedView();
    virtual ~blockedView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // BLOCKED_VIEW_HPP
