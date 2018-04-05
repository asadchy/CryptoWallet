#ifndef PIN_PRESENTER_HPP
#define PIN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class pinView;

class pinPresenter : public Presenter, public ModelListener
{
public:
    pinPresenter(pinView& v);

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

    virtual ~pinPresenter() {};

    virtual void setHeadText(touchgfx::Unicode::UnicodeChar *text);
    void setMnemonicSeed(Unicode::UnicodeChar *mnemonic);
    void pinScreenEntered();
    void showPinKeyboard();
    void showMSKeyboard();
    void showMSWindow();

    void pincodeEntered(int pincode);
    void msEntered(Unicode::UnicodeChar *mnemonic);
    void cancelPressed();
    void closePressed();

    virtual void toInitScreen();
    virtual void toStatusScreen();
    virtual void toPinScreen();

private:
    pinPresenter();

    pinView& view;
};


#endif // PIN_PRESENTER_HPP
