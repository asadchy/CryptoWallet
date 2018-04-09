#ifndef INIT_PRESENTER_HPP
#define INIT_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class initView;

class initPresenter : public Presenter, public ModelListener
{
public:
    initPresenter(initView& v);

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

    virtual ~initPresenter() {};

    virtual void toPinScreen();

    void restoreWallet();
    void initWallet();

private:
    initPresenter();

    initView& view;
};


#endif // INIT_PRESENTER_HPP
