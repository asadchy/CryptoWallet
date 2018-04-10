#ifndef STATUS_PRESENTER_HPP
#define STATUS_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <data.hpp>

using namespace touchgfx;

class statusView;

class statusPresenter : public Presenter, public ModelListener
{
public:
    statusPresenter(statusView& v);

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

    virtual ~statusPresenter() {};

    virtual void toMainScreen();

    virtual void toInitScreen();

    virtual void toStatusScreen();

    virtual void toBlockedScreen();

    void statusScreenEntered();

    void clearWallet();

    virtual void setDialogText(touchgfx::Unicode::UnicodeChar *text);

    virtual void walletStatus(struct wallet_status *status);

private:
    statusPresenter();

    statusView& view;
};


#endif // STATUS_PRESENTER_HPP
