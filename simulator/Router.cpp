#include "Router.h"

Router::Router(const int routerID)
	:
	m_routerID{ routerID } {
}

Router::~Router()
{
	for (auto& port : m_ports)
	{
		delete port;
		port = nullptr;
	}
}

void Router::runOneCycle()
{
	receiveFlit();
	receiveCredit();
	computeRoute();
	allocateVirtualChannel();
	traverseSwitch();
	allocateSwitch();
	debug();
}

Port* Router::createPort(const int portID)
{
	Port* port{ new Port{portID} };
	for (int i{}; i < g_virtualChannelNumber; ++i)
		port->m_controlFields.at(i).m_routedOutputPort = m_routerID;
	m_ports.push_back(port);
	return port;
}

void Router::updateEnable()
{
	updatePortInputRegisterEnable();
	resetVirtualChannelEnable();
}

void Router::updatePortInputRegisterEnable()
{
	for (auto& port : m_ports)
	{
		if (port->m_inputRegister.isFlitRegisterEmpty())
			port->m_inputRegister.m_flitEnable = false;
		else
			port->m_inputRegister.m_flitEnable = true;

		if (port->m_inputRegister.isCreditRegisterEmpty())
			port->m_inputRegister.m_creditEnable = false;
		else
			port->m_inputRegister.m_creditEnable = true;
	}
}

void Router::resetVirtualChannelEnable()
{
	for (auto& port : m_ports)
	{
		for (auto& controlField : port->m_controlFields)
			controlField.m_enable = true;
	}
}

void Router::initiatePriorities()
{
	m_priorityTableVA.clear();
	m_priorityTableSA.clear();
	for (int i{}; i < m_ports.size(); ++i)
	{
		for (int j{}; j < g_virtualChannelNumber; ++j)
		{
			m_priorityTableVA.push_back({ i, j });
			m_priorityTableSA.push_back({ i, j });
		}
	}
}

void Router::receiveFlit()
{
	for (auto& port : m_ports)
	{
		if (port->m_inputRegister.m_flitEnable)
		{
			Flit flit{ port->m_inputRegister.popfrontFlit() };
			port->m_virtualChannels.
				at(flit.m_flitVirtualChannel).push_back(flit);
			if (port->m_controlFields.at(flit.m_flitVirtualChannel)
				.m_virtualChannelState == VirtualChannelState::I)
				port->m_controlFields.at(flit.m_flitVirtualChannel)
				.m_virtualChannelState = VirtualChannelState::R;
			if (port->m_controlFields.at(flit.m_flitVirtualChannel)
				.m_virtualChannelState == VirtualChannelState::F)
				port->m_controlFields.at(flit.m_flitVirtualChannel)
				.m_virtualChannelState = VirtualChannelState::A;
		}
	}
}

void Router::receiveCredit()
{
	for (auto& port : m_ports)
	{
		if (port->m_inputRegister.m_creditEnable)
		{
			Credit credit{ port->m_inputRegister.popfrontCredit() };
			port->m_controlFields
				.at(credit.m_creditVirtualChannel).m_credit++;
			if (port->m_controlFields.at(credit.m_creditVirtualChannel)
				.m_downstreamVirtualChannelState == VirtualChannelState::C)
				port->m_controlFields.at(credit.m_creditVirtualChannel)
				.m_downstreamVirtualChannelState = VirtualChannelState::A;
			if (credit.m_isTail)
				port->m_controlFields.at(credit.m_creditVirtualChannel)
				.m_downstreamVirtualChannelState = VirtualChannelState::I;
		}
	}
}

void Router::computeRoute()
{
	for (auto& port : m_ports)
	{
		for (int i{}; i < g_virtualChannelNumber; ++i)
		{
			if (port->m_controlFields.at(i).m_virtualChannelState
				== VirtualChannelState::R &&
				port->m_controlFields.at(i).m_enable
				== true)
			{
				port->m_controlFields.at(i).m_routedOutputPort =
					port->m_virtualChannels.at(i).front()
					.m_route.front();
				// do not pop front the last element in the route, 
				// it is the destination
				if (port->m_virtualChannels.at(i).front()
					.m_route.front() >= 0)
					port->m_virtualChannels.at(i).front()
					.m_route.pop_front();
				port->m_controlFields.at(i).m_virtualChannelState =
					VirtualChannelState::V;
				port->m_controlFields.at(i).m_enable = false;
			}
		}
	}
}

void Router::allocateVirtualChannel()
{
	// round-robin: record arbitration winners
	std::vector<PriorityTableEntry> winners{};

	// check by priority table VA
	for (auto& entry : m_priorityTableVA)
	{
		if (m_ports.at(entry.m_portIndex)
			->m_controlFields.at(entry.m_virtualChannelIndex)
			.m_virtualChannelState == VirtualChannelState::V &&
			m_ports.at(entry.m_portIndex)
			->m_controlFields.at(entry.m_virtualChannelIndex)
			.m_enable == true)
		{
			for (auto& port : m_ports)
			{
				// find the output port that is routed
				if (m_ports.at(entry.m_portIndex)
					->m_controlFields.at(entry.m_virtualChannelIndex)
					.m_routedOutputPort == port->m_portID)
				{
					for (int i{}; i < g_virtualChannelNumber; ++i)
					{
						// find the Idle downstream virtual channel
						if (port->m_controlFields.at(i)
							.m_downstreamVirtualChannelState
							== VirtualChannelState::I)
						{
							// change input
							m_ports.at(entry.m_portIndex)->m_controlFields
								.at(entry.m_virtualChannelIndex)
								.m_allocatedVirtualChannel = i;
							m_ports.at(entry.m_portIndex)->m_controlFields
								.at(entry.m_virtualChannelIndex)
								.m_virtualChannelState
								= VirtualChannelState::A;
							// change output
							port->m_controlFields.at(i)
								.m_downstreamVirtualChannelState
								= VirtualChannelState::A;
							// round-robin: push entry into winners
							winners.push_back(entry);
							m_ports.at(entry.m_portIndex)->m_controlFields
								.at(entry.m_virtualChannelIndex)
								.m_enable = false;
							break;
						}
					}
					break;
				}
			}
		}
	}

	// round-robin: winners will have the lowest priorities in next round
	for (auto& entry : winners)
	{
		std::erase(m_priorityTableVA, entry);
		m_priorityTableVA.push_back(entry);
	}
}

bool Router::checkConflict(const int inputPortIndex,
	const int outputPortIndex)
{
	for (auto& connection : m_crossbar)
	{
		if (connection.m_inputPortIndex == inputPortIndex ||
			connection.m_outputPortIndex == outputPortIndex)
			return false; // conflict
	}
	return true; // no conflict
}

void Router::allocateSwitch()
{
	// round-robin: record arbitration winners
	std::vector<PriorityTableEntry> winners{};

	// check by priority table SA
	for (auto& entry : m_priorityTableSA)
	{
		if (m_ports.at(entry.m_portIndex)
			->m_controlFields.at(entry.m_virtualChannelIndex)
			.m_virtualChannelState == VirtualChannelState::A &&
			m_ports.at(entry.m_portIndex)
			->m_controlFields.at(entry.m_virtualChannelIndex)
			.m_enable == true)
		{
			// i is output port index
			for (int i{}; i < m_ports.size(); ++i)
			{
				// find the output port that is routed
				// and check if the downstream virtual channel
				// state is still Active
				if (m_ports.at(entry.m_portIndex)
					->m_controlFields.at(entry.m_virtualChannelIndex)
					.m_routedOutputPort
					== m_ports.at(i)->m_portID &&
					m_ports.at(i)->m_controlFields
					.at(m_ports.at(entry.m_portIndex)
						->m_controlFields
						.at(entry.m_virtualChannelIndex)
						.m_allocatedVirtualChannel)
					.m_downstreamVirtualChannelState
					== VirtualChannelState::A)
				{
					// if no conflict, 
					// add this connection into crossbar
					if (checkConflict(entry.m_portIndex, i))
						m_crossbar.push_back({
							entry.m_portIndex,
							entry.m_virtualChannelIndex,
							i,
							m_ports.at(entry.m_portIndex)
							->m_controlFields
							.at(entry.m_virtualChannelIndex)
							.m_allocatedVirtualChannel });
					// round-robin: push entry into winners
					winners.push_back(entry);
					m_ports.at(entry.m_portIndex)
						->m_controlFields.at(entry.m_virtualChannelIndex)
						.m_enable = false;
					break;
				}
			}
		}
	}

	// round-robin: winners will have the lowest priorities in next round
	for (auto& entry : winners)
	{
		std::erase(m_priorityTableSA, entry);
		m_priorityTableSA.push_back(entry);
	}
}

void Router::traverseSwitch()
{
	// transmit flits and credits
	for (auto& connection : m_crossbar)
	{
		// read flit out
		Flit flit{ m_ports.at(connection.m_inputPortIndex)
			->m_virtualChannels.at(connection.m_inputVirtualChannelIndex)
			.front() };
		// change flit virtual channel field
		flit.m_flitVirtualChannel = connection.m_outputVirtualChannelIndex;
		// push flit into output port output register
		m_ports.at(connection.m_outputPortIndex)->m_outputRegister
			.pushbackFlit(flit);
		// decrement output port virtual channel credit
		// do not do this if output port is terminal port
		if (m_ports.at(connection.m_outputPortIndex)->m_portID >= 0)
			m_ports.at(connection.m_outputPortIndex)
			->m_controlFields.at(connection.m_outputVirtualChannelIndex)
			.m_credit--;
		// change output port downstream virtual channel state to C 
		// if output port virtual channel credit is zero
		if (!m_ports.at(connection.m_outputPortIndex)
			->m_controlFields.at(connection.m_outputVirtualChannelIndex)
			.m_credit)
			m_ports.at(connection.m_outputPortIndex)
			->m_controlFields.at(connection.m_outputVirtualChannelIndex)
			.m_downstreamVirtualChannelState = VirtualChannelState::C;
		// pop out flit
		m_ports.at(connection.m_inputPortIndex)
			->m_virtualChannels.at(connection.m_inputVirtualChannelIndex)
			.pop_front();
		// change input port virtual channel state to F
		// if input port virtual channel is empty
		if (m_ports.at(connection.m_inputPortIndex)
			->m_virtualChannels.at(connection.m_inputVirtualChannelIndex)
			.empty())
			m_ports.at(connection.m_inputPortIndex)
			->m_controlFields.at(connection.m_inputVirtualChannelIndex)
			.m_virtualChannelState = VirtualChannelState::F;
		// create a credit with input port virtual channel
		// credit should have a bit telling if it is tail flit
		// if it is, downstream virtual channel state will be reset to I
		Credit credit{ connection.m_inputVirtualChannelIndex, false };
		if (flit.m_flitType == FlitType::T)
			credit.m_isTail = true;
		// push credit into input port output register
		m_ports.at(connection.m_inputPortIndex)
			->m_outputRegister.pushbackCredit(credit);
		// reset input port virtual channel input fields
		if (flit.m_flitType == FlitType::T)
		{
			m_ports.at(connection.m_inputPortIndex)
				->m_controlFields.at(connection.m_inputVirtualChannelIndex)
				.m_virtualChannelState = VirtualChannelState::I;
			m_ports.at(connection.m_inputPortIndex)
				->m_controlFields.at(connection.m_inputVirtualChannelIndex)
				.m_routedOutputPort = m_routerID;
			m_ports.at(connection.m_inputPortIndex)
				->m_controlFields.at(connection.m_inputVirtualChannelIndex)
				.m_allocatedVirtualChannel = -1;
			// reset output port downstream virtual channel state,
			// if output port is terminal port
			if (m_ports.at(connection.m_outputPortIndex)->m_portID < 0)
				m_ports.at(connection.m_outputPortIndex)
				->m_controlFields.at(connection.m_outputVirtualChannelIndex)
				.m_downstreamVirtualChannelState = VirtualChannelState::I;
		}
	}

	// cutoff crossbar connections
	m_crossbar.clear();
}

void Router::debug()
{
#if (DEBUG > 0)
	std::cout << "----------------"
		<< "Router ID "
		<< m_routerID
		<< "----------------"
		<< std::endl;
	for (auto& port : m_ports)
	{
		std::cout << "------------"
			<< "Port ID "
			<< port->m_portID
			<< "------------"
			<< std::endl;
		std::cout << "Input--------------" << std::endl;
		port->m_inputRegister.debug();
		for (int i{}; i < g_virtualChannelNumber; ++i)
		{
			std::cout << "Virtual Channel " << i << "| ";
			std::cout << port->m_controlFields.at(i)
				.m_virtualChannelState << "|";
			std::cout << port->m_controlFields.at(i)
				.m_routedOutputPort << "|";
			std::cout << port->m_controlFields.at(i)
				.m_allocatedVirtualChannel << std::endl;
			std::cout << "Data| ";
			for (auto& buffer : port->m_virtualChannels.at(i))
				std::cout << buffer.m_flitType
				<< buffer.m_flitNumberB << "|";
			std::cout << std::endl;
		}
		std::cout << "Output--------------" << std::endl;
		port->m_outputRegister.debug();
		for (int i{}; i < g_virtualChannelNumber; ++i)
		{
			std::cout << "Downstream Virtual Channel " << i << "| ";
			std::cout << port->m_controlFields.at(i)
				.m_downstreamVirtualChannelState << "|";
			std::cout << port->m_controlFields.at(i)
				.m_credit << "|" << std::endl;
		}
	}
#endif
}