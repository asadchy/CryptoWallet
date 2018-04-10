#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent()
    {
        model.tick();
        FrontendApplicationBase::handleTickEvent();
    }

    void gotoMainScreen();
    void gotoStatusScreen();
    void gotoPinScreen();
    void gotoInitScreen();

private:
    void gotoMainScreenImpl();
	void gotoStatusScreenImpl();
	void gotoPinScreenImpl();
	void gotoInitScreenImpl();
    touchgfx::Callback<FrontendApplication> customTransitionCallback;
};

#endif // FRONTENDAPPLICATION_HPP
