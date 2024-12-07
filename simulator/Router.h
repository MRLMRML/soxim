#pragma once
#include "DataStructures.h"
#include "Port.h"

class Router
{
public:
	Router() = default;
	Router(const int routerID);
	~Router();

	void runOneCycle();
	Port* createPort(const int portID);
	void updateEnable();
	void initiatePriorities();

private:
	void updatePortInputRegisterEnable(); // update port input registers enable
	void resetVirtualChannelEnable(); // reset virtual channel enable
	void receiveFlit();
	void receiveCredit();

	void computeRoute();
	void allocateVirtualChannel();
	void allocateSwitch();
	void traverseSwitch();

	bool checkConflict(const int inputPort,
		const int outputPort);

	void debug();

public:
	int m_routerID{}; // ID starts from 0, 1, 2, ...
	std::vector<Port*> m_ports{};

private:
	std::vector<Connection> m_crossbar{};
	std::vector<PriorityTableEntry> m_priorityTableVA{}; // priority for VA
	std::vector<PriorityTableEntry> m_priorityTableSA{}; // priority for SA
};