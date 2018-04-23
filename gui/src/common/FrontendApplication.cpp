#include <gui/common/FrontendApplication.hpp>

#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <gui/startup_screen/startupView.hpp>
#include <gui/startup_screen/startupPresenter.hpp>
#include <gui/transaction_screen/transactionView.hpp>
#include <gui/transaction_screen/transactionPresenter.hpp>
#include <gui/blocked_screen/blockedView.hpp>
#include <gui/blocked_screen/blockedPresenter.hpp>

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
	/*
	makeTransition<transactionView, transactionPresenter, CustomTransition, Model>(&currentScreen,
			&currentPresenter, frontendHeap, &currentTransition, &model);
	*/
	makeTransition<transactionView, transactionPresenter, NoTransition, Model>(&currentScreen,
				&currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoStatusScreen()
{
	customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoStatusScreenImpl);
	pendingScreenTransitionCallback = &customTransitionCallback;
}

void FrontendApplication::gotoStatusScreenImpl()
{
	/*
	makeTransition<statusView, statusPresenter, CustomTransition, Model>(&currentScreen,
				&currentPresenter, frontendHeap, &currentTransition, &model);
	*/
	makeTransition<statusView, statusPresenter, NoTransition, Model>(&currentScreen,
					&currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoPinScreen()
{
	customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoPinScreenImpl);
	pendingScreenTransitionCallback = &customTransitionCallback;
}

void FrontendApplication::gotoPinScreenImpl()
{
	/*
	makeTransition<pinView, pinPresenter, CustomTransition, Model>(&currentScreen,
				&currentPresenter, frontendHeap, &currentTransition, &model);
	*/
	makeTransition<pinView, pinPresenter, NoTransition, Model>(&currentScreen,
					&currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoInitScreen()
{
	customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoInitScreenImpl);
	pendingScreenTransitionCallback = &customTransitionCallback;
}

void FrontendApplication::gotoInitScreenImpl()
{
	/*
	makeTransition<initView, initPresenter, CustomTransition, Model>(&currentScreen,
				&currentPresenter, frontendHeap, &currentTransition, &model);
	*/
	makeTransition<initView, initPresenter, NoTransition, Model>(&currentScreen,
					&currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoBlockedScreen()
{
	customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoBlockedScreenImpl);
	pendingScreenTransitionCallback = &customTransitionCallback;
}

void FrontendApplication::gotoBlockedScreenImpl()
{
	makeTransition<blockedView, blockedPresenter, NoTransition, Model>(&currentScreen,
					&currentPresenter, frontendHeap, &currentTransition, &model);
}
