/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/transaction_screen/transactionViewBase.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

transactionViewBase::transactionViewBase()  
{
    background.setXY(0, 0);
    background.setBitmap(Bitmap(BITMAP_BACKGROUND_3_ID));

    currency.setPosition(18, 62, 315, 28);
    currency.setColor(touchgfx::Color::getColorFrom24BitRGB(126,174,229));
    currency.setLinespacing(0);
    currency.setTypedText(TypedText(T_CURRENCY));
    currencyBuffer[0] = 0;
    currency.setWildcard(currencyBuffer);

    address.setPosition(18, 84, 444, 68);
    address.setColor(touchgfx::Color::getColorFrom24BitRGB(126,174,229));
    address.setLinespacing(0);
    address.setTypedText(TypedText(T_ADDRESS));
    addressBuffer[0] = 0;
    address.setWildcard(addressBuffer);

    value.setPosition(18, 130, 338, 35);
    value.setColor(touchgfx::Color::getColorFrom24BitRGB(126,174,229));
    value.setLinespacing(0);
    value.setTypedText(TypedText(T_VALUE));
    valueBuffer[0] = 0;
    value.setWildcard(valueBuffer);

    confirm.setPosition(364, 126, 96, 40);
    confirm.setBitmaps(Bitmap(BITMAP_BUTTON_NOT_PRESSED_ID), Bitmap(BITMAP_BUTTON_PRESSED_ID));
    confirm.setLabelText(TypedText(T_CONFIRM));
    confirm.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(126,174,229));
    confirm.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(126,174,229));

    cancel.setPosition(364, 179, 96, 40);
    cancel.setBitmaps(Bitmap(BITMAP_BUTTON_NOT_PRESSED_ID), Bitmap(BITMAP_BUTTON_PRESSED_ID));
    cancel.setLabelText(TypedText(T_CANCEL));
    cancel.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(126,174,229));
    cancel.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(126,174,229));

    transaction.setXY(30, 20);
    transaction.setColor(touchgfx::Color::getColorFrom24BitRGB(126,174,229));
    transaction.setLinespacing(0);
    transaction.setTypedText(TypedText(T_CURR_TRANSACTION));
    add(background);
    add(currency);
    add(address);
    add(value);
    add(confirm);
    add(cancel);
    add(transaction);
}
