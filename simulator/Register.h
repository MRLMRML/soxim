#pragma once
#include "DataStructures.h"

class Register
{
public:
	Register() = default;
	void pushbackFlit(const Flit flit);
	void pushbackCredit(const Credit credit);
	Flit popfrontFlit();
	Credit popfrontCredit();
	bool isFlitRegisterEmpty();
	bool isCreditRegisterEmpty();
	void debug();

	bool m_flitEnable{};
	bool m_creditEnable{};

private:
	std::deque<Flit> m_flitRegister{};
	std::deque<Credit> m_creditRegister{};
};