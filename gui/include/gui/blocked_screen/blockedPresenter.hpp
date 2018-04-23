#ifndef BLOCKED_PRESENTER_HPP
#define BLOCKED_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class blockedView;

class blockedPresenter : public Presenter, public ModelListener
{
public:
    blockedPresenter(blockedView& v);

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

    virtual ~blockedPresenter() {};

    virtual void toBlockedScreen();
    virtual void toInitScreen();
    virtual void toPinScreen();
    virtual void toStatusScreen();
    virtual void toMainScreen();

private:
    blockedPresenter();

    blockedView& view;
};


#endif // BLOCKED_PRESENTER_HPP
