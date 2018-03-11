#ifndef TRANSACTION_PRESENTER_HPP
#define TRANSACTION_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class transactionView;

class transactionPresenter : public Presenter, public ModelListener
{
public:
    transactionPresenter(transactionView& v);

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

    virtual ~transactionPresenter() {};

    virtual void setCurrency(char *currency);
	virtual void setAddress(char *addr);
	virtual void setValue(double value);

	void transactionScreenEntered();
	virtual void setDialogText(touchgfx::Unicode::UnicodeChar *text);

	void cancelPressed();
	void confirmPressed();

	virtual void toPincodeScreen();
	virtual void toStatusScreen();

private:
    transactionPresenter();

    transactionView& view;
};


#endif // TRANSACTION_PRESENTER_HPP
