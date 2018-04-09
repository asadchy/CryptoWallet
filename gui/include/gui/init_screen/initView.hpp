#ifndef INIT_VIEW_HPP
#define INIT_VIEW_HPP

#include <gui_generated/init_screen/initViewBase.hpp>
#include <gui/init_screen/initPresenter.hpp>

class initView : public initViewBase
{
public:
    initView();
    virtual ~initView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
private:
    void buttonPressedHandler(const touchgfx::AbstractButton& btn);
    touchgfx::Callback<initView, const touchgfx::AbstractButton&> buttonPressed;
};

#endif // INIT_VIEW_HPP
