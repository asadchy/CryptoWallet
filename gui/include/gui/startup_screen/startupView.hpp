#ifndef STARTUP_VIEW_HPP
#define STARTUP_VIEW_HPP

#include <gui_generated/startup_screen/startupViewBase.hpp>
#include <gui/startup_screen/startupPresenter.hpp>

class startupView : public startupViewBase
{
public:
    startupView();
    virtual ~startupView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // STARTUP_VIEW_HPP
