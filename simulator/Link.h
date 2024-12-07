#pragma once
#include "Router.h"
#include "TerminalInterface.h"

class Link
{
public:
	Link(Router* leftRouter, Router* rightRouter);
	Link(Router* leftRouter,
		TerminalInterface* rightTerminalInterface);

	void runOneCycle();
	void updateEnable();

private:

	Router* m_leftRouter{};
	Router* m_rightRouter{};
	TerminalInterface* m_rightTerminalInterface{};
	Port* m_leftPort{};
	Port* m_rightPort{};
};