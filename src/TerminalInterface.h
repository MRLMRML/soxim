#pragma once
#include "DataStructures.h"
#include "Port.h"
#include "Clock.h"

class TerminalInterface
{
public:
	TerminalInterface(const int terminalInterfaceID);

	Port* getPort(const int portID);
	void updateEnable(); // update port input registers enable
	void runOneCycle();
	bool operator==(
		const TerminalInterface& terminalInterface) const;

private:
	// read packet from files, make filts,
	// and push them into source queue
	void injectTraffic();
	void readPacket();
	void makeFlits(const Packet& packet);
	std::deque<int> getRoute(const int destination);

	// send flit out from source queue
	void sendFlit();
	bool allocateVirtualChannel();
	void moveFlitOut();

	// receive credit
	void receiveCredit();

	// receive flits into reorder buffer,
	// make packet, and write packet to files
	void receiveFlit();
	void makePacket(const Flit& flit);
	void writePacket(const Packet& packet);

public:
	Clock m_clock{};
	int m_terminalInterfaceID{}; // ID starts from -1, -2, ...
	Coordinate m_terminalInterfaceIDTorus{}; // (x, y, z) ID in Torus network, converted from Router ID
	Port m_port{}; // port ID is the same as the Router ID that it connects to
	std::vector<std::deque<int>> m_sourceRoutingTable{}; // the back() element is the destination terminal interface ID
	std::deque<Flit> m_sourceQueue{};
	std::vector<Flit> m_reorderBuffer{};
	std::vector<TrafficInformationEntry> m_outputTrafficInfoBuffer{};
	std::vector<std::vector<float>> m_outputTrafficDataBuffer{};
	std::vector<TrafficInformationEntry> m_inputTrafficInfoBuffer{};
	std::vector<std::vector<float>> m_inputTrafficDataBuffer{};
};