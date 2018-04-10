#ifndef STARTUP_PRESENTER_HPP
#define STARTUP_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class startupView;

class startupPresenter : public Presenter, public ModelListener
{
public:
    startupPresenter(startupView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~startupPresenter() {};

    virtual void toStatusScreen();
    virtual void toPinScreen();
    virtual void toInitScreen();

private:
    startupPresenter();

    startupView& view;
};


#endif // STARTUP_PRESENTER_HPP
