#include <gui/common/FrontendApplication.hpp>

#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <gui/pin_code_screen/pin_codeView.hpp>
#include <gui/pin_code_screen/pin_codePresenter.hpp>
#include <gui/startup_screen/startupView.hpp>
#include <gui/startup_screen/startupPresenter.hpp>
#include <gui/transaction_screen/transactionView.hpp>
#include <gui/transaction_screen/transactionPresenter.hpp>
#include <gui/common/CustomTransition.hpp>


FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{
}

void FrontendApplication::gotoMainScreen()
{
	customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoMainScreenImpl);
	pendingScreenTransitionCallback = &customTransitionCallback;
}

void FrontendApplication::gotoMainScreenImpl()
{
	makeTransition<transactionView, transactionPresenter, CustomTransition, Model>(&currentScreen,
			&currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoPincodeScreen()
{
	customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoPincodeScreenImpl);
	pendingScreenTransitionCallback = &customTransitionCallback;
}

void FrontendApplication::gotoPincodeScreenImpl()
{
	makeTransition<pin_codeView, pin_codePresenter, CustomTransition, Model>(&currentScreen,
				&currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoStatusScreen()
{
	customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoStatusScreenImpl);
	pendingScreenTransitionCallback = &customTransitionCallback;
}

void FrontendApplication::gotoStatusScreenImpl()
{
	makeTransition<statusView, statusPresenter, CustomTransition, Model>(&currentScreen,
				&currentPresenter, frontendHeap, &currentTransition, &model);
}
