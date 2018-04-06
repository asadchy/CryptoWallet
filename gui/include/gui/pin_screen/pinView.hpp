#ifndef PIN_VIEW_HPP
#define PIN_VIEW_HPP

#include <gui_generated/pin_screen/pinViewBase.hpp>
#include <gui/pin_screen/pinPresenter.hpp>
#include <gui/common/PinKeyboard.hpp>
#include <gui/common/MSKeyboard.hpp>
#include <gui/common/dialog.hpp>

class pinView : public pinViewBase
{
public:
    pinView();
    virtual ~pinView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void showPinKeyboard();
    virtual void showMSKeyboard();
    virtual void showMSWindow();

    void setHeadText(touchgfx::Unicode::UnicodeChar *text);
    void setMnemonicSeed(Unicode::UnicodeChar *mnemonic);
    void pinScreenEntered();
    void pincodeEntered(int pincode);
    void MSEntered(Unicode::UnicodeChar *mnemonic);
    void cancelPressed();
    void closePressed();
private:
    PinKeyboard pinKeyboard;
    MSKeyboard msKeyboard;
    Dialog dialog;

    Callback<pinView, int> pincodeEnteredCallback;
    Callback<pinView, Unicode::UnicodeChar*> MSEnteredCallback;
    Callback<pinView, void> cancelPressedCallback;
    void closePressedHandler(const touchgfx::AbstractButton& btn);
	touchgfx::Callback<pinView, const touchgfx::AbstractButton&> closePressedCallback;

    Image msWindow;
    ButtonWithLabel closeBtn;
    TextAreaWithOneWildcard mnemonicSeed;
	static const uint16_t MNEMONICSEED_SIZE = 123;
	touchgfx::Unicode::UnicodeChar mnemonicSeedBuffer[MNEMONICSEED_SIZE];
};

#endif // PIN_VIEW_HPP
