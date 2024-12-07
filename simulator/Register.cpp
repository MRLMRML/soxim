#include "Register.h"

void Register::pushbackFlit(const Flit flit)
{
	m_flitRegister.push_back(flit);
}

void Register::pushbackCredit(const Credit credit)
{
	m_creditRegister.push_back(credit);
}

Flit Register::popfrontFlit()
{
	Flit flit{ m_flitRegister.front() };
	m_flitRegister.pop_front();
	return flit;
}

Credit Register::popfrontCredit()
{
	Credit credit{ m_creditRegister.front() };
	m_creditRegister.pop_front();
	return credit;
}

bool Register::isFlitRegisterEmpty()
{
	return m_flitRegister.empty();
}

bool Register::isCreditRegisterEmpty()
{
	return m_creditRegister.empty();
}

void Register::debug()
{
#if (DEBUG > 0)
	std::cout << "Flit Register: ";
	for (auto& flit : m_flitRegister)
		std::cout << flit.m_flitType
		<< flit.m_flitNumberB << "|";
	std::cout << std::endl;

	std::cout << "Credit Register: ";
	for (auto& credit : m_creditRegister)
		std::cout << credit.m_creditVirtualChannel << " "
		<< credit.m_isTail << "|";
	std::cout << std::endl;
#endif
}