#pragma once
#include "DataStructures.h"
#include "Register.h"

struct Port
{
	Port() = default;
	Port(const int portID)
		:
		m_portID{ portID } {
	}

	int m_portID{}; // the same as the routerID that this port connects to
	Register m_inputRegister{}, m_outputRegister{};
	std::vector<std::deque<Flit>> m_virtualChannels{ std::vector<std::deque<Flit>>(g_virtualChannelNumber) };
	std::vector<ControlField> m_controlFields{ std::vector<ControlField>(g_virtualChannelNumber) };
};