#pragma once
#include "Link.h"

class RegularNetwork
{
public:
	RegularNetwork();
	~RegularNetwork(); // release routers, links, and terminals

	void runOneCycle();
	int getRouterNumber();
	void connectTerminal(const int routerID,
		TerminalInterface* terminalInterface);
	void loadNetworkData();

private:
	void generateRoutes();
	void updatePriorities();
	void createRouters();
	void deleteRouters();
	void connectMESH();
	void connectTORUS();
	void deleteLinks();
	void deleteTerminalInterfaces();
	Coordinate convertIDToCoordinate(const int id);
	int convertCoordinateToID(const Coordinate& coordinate);
	void routeDOR();
	void routeROMM(); // Randomized Oblivious Multi-phase Minimal
	void routeMAD();  // Minimal Adaptive
	void routeVAL();  // Valiant's Randomized Algorithm
	void routeOddEven(); // Odd-Even Adaptive

public:
	std::vector<TerminalInterface*> m_terminalInterfaces{};

private:
	Coordinate m_dimension{g_x, g_y, g_z};
	std::vector<Router*> m_routers{};
	std::vector<Link*> m_links{};
};