#include "TerminalInterface.h"

TerminalInterface::TerminalInterface(const int terminalInterfaceID)
	:
	m_terminalInterfaceID{ terminalInterfaceID }
{
	std::random_device rd;
	std::mt19937 gen(rd());  // to seed mersenne twister
	std::uniform_int_distribution<> dist(0, 10);
	m_clock.set(static_cast<float>(dist(gen)));
	//m_clock.set(0);
}

Port* TerminalInterface::getPort(const int portID)
{
	m_port.m_portID = portID;
	return &m_port;
}

void TerminalInterface::updateEnable()
{
	if (m_port.m_inputRegister.isFlitRegisterEmpty())
		m_port.m_inputRegister.m_flitEnable = false;
	else
		m_port.m_inputRegister.m_flitEnable = true;

	if (m_port.m_inputRegister.isCreditRegisterEmpty())
		m_port.m_inputRegister.m_creditEnable = false;
	else
		m_port.m_inputRegister.m_creditEnable = true;
}

void TerminalInterface::runOneCycle()
{
	injectTraffic();
	receiveCredit();
	sendFlit();
	receiveFlit();
}

bool TerminalInterface::operator==(
	const TerminalInterface& terminalInterface) const
{
	return m_terminalInterfaceID ==
		terminalInterface.m_terminalInterfaceID;
}

void TerminalInterface::injectTraffic()
{
	std::random_device rd;
	std::mt19937 gen(rd());  // to seed mersenne twister
	std::bernoulli_distribution distBernoulli(g_injectionRate);
	std::bernoulli_distribution distMMPOnState(g_alpha / (g_alpha + g_beta));

	if (g_injectionProcess == "periodic")
	{
		if (m_clock.trigger())
		{
			readPacket();
			m_clock.set(1 / g_injectionRate);
		}
	}
	else if (g_injectionProcess == "bernoulli")
	{
		if (distBernoulli(gen))
			readPacket();
	}
	else if (g_injectionProcess == "markov modulated process")
	{
		if (distMMPOnState(gen))
		{
			if (distBernoulli(gen))
				readPacket();
		}
	}
}

void TerminalInterface::readPacket()
{
	for (size_t i{}; i < m_outputTrafficInfoBuffer.size(); ++i)
	{
		if (m_outputTrafficInfoBuffer.at(i).m_status == "V")
		{
			m_outputTrafficInfoBuffer.at(i).m_status = "S";
			m_outputTrafficInfoBuffer.at(i).m_sentTime = m_clock.get();

			Packet packet{ m_outputTrafficInfoBuffer.at(i).m_packetID,
			m_outputTrafficInfoBuffer.at(i).m_source,
			m_outputTrafficInfoBuffer.at(i).m_destination,
			m_outputTrafficDataBuffer.at(i) };

			makeFlits(packet); // make flits and send it into source queue
			break;
		}
	}
}

void TerminalInterface::makeFlits(const Packet& packet)
{
	m_sourceQueue.push_back({ packet.m_source,
		getRoute(packet.m_destination) }); // H

	for (size_t i{}; i < packet.m_data.size(); i += static_cast<size_t>(g_flitSize)) // B
	{
		std::vector<float> flitData{};
		for (int j{}; j < g_flitSize; ++j)
			flitData.push_back(packet.m_data.at(i + j));
		m_sourceQueue.push_back({ flitData, static_cast<int>(i) });
	}

	m_sourceQueue.push_back({ packet.m_packetID }); // T
}

std::deque<int> TerminalInterface::getRoute(const int destination)
{
	for (auto& entry : m_sourceRoutingTable)
	{
		if (entry.back() == destination)
			return entry;
	}
	return {}; // Return empty deque if route not found
}

void TerminalInterface::sendFlit()
{
	if (!m_sourceQueue.empty())
	{
		if (m_sourceQueue.front().m_flitType == FlitType::H)
		{
			if (allocateVirtualChannel())
				moveFlitOut();
		}
		else
		{
			if (m_port.m_controlFields.at(
				m_port.m_controlFields.front().m_allocatedVirtualChannel)
				.m_downstreamVirtualChannelState
				== VirtualChannelState::A)
				moveFlitOut();
		}
	}
}

bool TerminalInterface::allocateVirtualChannel()
{
	for (int i{}; i < g_virtualChannelNumber; ++i)
	{
		if (m_port.m_controlFields.at(i).m_downstreamVirtualChannelState
			== VirtualChannelState::I)
		{
			// the first input control field allocated virtual channel
			// is used to record vc allocation result of source queue
			m_port.m_controlFields.front().m_allocatedVirtualChannel = i;
			m_port.m_controlFields.at(i).m_downstreamVirtualChannelState
				= VirtualChannelState::A;
			return true;
		}
	}
	return false;
}

void TerminalInterface::moveFlitOut()
{
	// read flit out
	Flit flit{ m_sourceQueue.front() };
	// change flit virtual channel field
	flit.m_flitVirtualChannel
		= m_port.m_controlFields.front().m_allocatedVirtualChannel;
	// push flit into output port output register
	m_port.m_outputRegister.pushbackFlit(flit);
	// decrement output port virtual channel credit
	m_port.m_controlFields.at(
		m_port.m_controlFields.front().m_allocatedVirtualChannel)
		.m_credit--;
	// change output port downstream virtual channel state to C 
	// if output port virtual channel credit is zero
	if (!m_port.m_controlFields.at(
		m_port.m_controlFields.front().m_allocatedVirtualChannel)
		.m_credit)
		m_port.m_controlFields.at(
			m_port.m_controlFields.front().m_allocatedVirtualChannel)
		.m_downstreamVirtualChannelState = VirtualChannelState::C;
	// pop out flit
	m_sourceQueue.pop_front();
}

void TerminalInterface::receiveCredit()
{
	if (m_port.m_inputRegister.m_creditEnable)
	{
		Credit credit{ m_port.m_inputRegister.popfrontCredit() };
		m_port.m_controlFields
			.at(credit.m_creditVirtualChannel).m_credit++;
		if (m_port.m_controlFields.at(credit.m_creditVirtualChannel)
			.m_downstreamVirtualChannelState == VirtualChannelState::C)
			m_port.m_controlFields.at(credit.m_creditVirtualChannel)
			.m_downstreamVirtualChannelState = VirtualChannelState::A;
		if (credit.m_isTail)
			m_port.m_controlFields.at(credit.m_creditVirtualChannel)
			.m_downstreamVirtualChannelState = VirtualChannelState::I;
	}
}

void TerminalInterface::receiveFlit()
{
	if (m_port.m_inputRegister.m_flitEnable)
	{
		Flit flit{ m_port.m_inputRegister.popfrontFlit() };
		m_reorderBuffer.push_back(flit);
		if (flit.m_flitType == FlitType::T)
			makePacket(flit);
	}
}

void TerminalInterface::makePacket(const Flit& flit)
{
	Packet packet{};
	std::vector<Flit> t_reorderBuffer{ m_reorderBuffer };
	for (auto& entry : t_reorderBuffer)
	{
		if (entry.m_flitVirtualChannel == flit.m_flitVirtualChannel)
		{
			switch (entry.m_flitType)
			{
			case FlitType::H:
				packet.m_source = entry.m_source;
				packet.m_destination = entry.m_route.back();
				break;
			case FlitType::B:
				for (auto& data : entry.m_flitData)
					packet.m_data.push_back(data);
				break;
			case FlitType::T:
				packet.m_packetID = entry.m_packetID;
				break;
			}
			std::erase(m_reorderBuffer, entry);
		}
	}
	writePacket(packet);
}

void TerminalInterface::writePacket(const Packet& packet)
{
	m_inputTrafficInfoBuffer.push_back({ packet.m_packetID,
	packet.m_source, packet.m_destination, static_cast<int>(packet.m_data.size()),
	"R", 0, m_clock.get() });

	m_inputTrafficDataBuffer.push_back(packet.m_data);
}
