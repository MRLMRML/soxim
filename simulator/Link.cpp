#include "Link.h"

Link::Link(Router* leftRouter,
	Router* rightRouter)
	:
	m_leftRouter{ leftRouter },
	m_rightRouter{ rightRouter }
{
	m_leftPort =
		m_leftRouter->createPort(m_rightRouter->m_routerID);
	m_rightPort =
		m_rightRouter->createPort(m_leftRouter->m_routerID);
}

Link::Link(Router* leftRouter,
	TerminalInterface* rightTerminalInterface)
	:
	m_leftRouter{ leftRouter },
	m_rightTerminalInterface{ rightTerminalInterface }
{
	m_leftPort =
		m_leftRouter->createPort(m_rightTerminalInterface
			->m_terminalInterfaceID);
	m_rightPort =
		m_rightTerminalInterface->getPort(m_leftRouter
			->m_routerID);
}

void Link::runOneCycle()
{
	if (m_leftPort->m_outputRegister.m_flitEnable)
		m_rightPort->m_inputRegister.pushbackFlit(
			m_leftPort->m_outputRegister.popfrontFlit());
	if (m_leftPort->m_outputRegister.m_creditEnable)
		m_rightPort->m_inputRegister.pushbackCredit(
			m_leftPort->m_outputRegister.popfrontCredit());
	if (m_rightPort->m_outputRegister.m_flitEnable)
		m_leftPort->m_inputRegister.pushbackFlit(
			m_rightPort->m_outputRegister.popfrontFlit());
	if (m_rightPort->m_outputRegister.m_creditEnable)
		m_leftPort->m_inputRegister.pushbackCredit(
			m_rightPort->m_outputRegister.popfrontCredit());
}

void Link::updateEnable()
{
	if (m_leftPort->m_outputRegister.isFlitRegisterEmpty())
		m_leftPort->m_outputRegister.m_flitEnable = false;
	else
		m_leftPort->m_outputRegister.m_flitEnable = true;
	if (m_leftPort->m_outputRegister.isCreditRegisterEmpty())
		m_leftPort->m_outputRegister.m_creditEnable = false;
	else
		m_leftPort->m_outputRegister.m_creditEnable = true;
	if (m_rightPort->m_outputRegister.isFlitRegisterEmpty())
		m_rightPort->m_outputRegister.m_flitEnable = false;
	else
		m_rightPort->m_outputRegister.m_flitEnable = true;
	if (m_rightPort->m_outputRegister.isCreditRegisterEmpty())
		m_rightPort->m_outputRegister.m_creditEnable = false;
	else
		m_rightPort->m_outputRegister.m_creditEnable = true;
}