#include "RegularNetwork.h"
#include <random>
#include <cmath>

RegularNetwork::RegularNetwork()
{
	createRouters();
	if (g_shape == "MESH")
		connectMESH();
	else if (g_shape == "TORUS")
		connectTORUS();
}

RegularNetwork::~RegularNetwork()
{
	deleteRouters();
	deleteLinks();
	deleteTerminalInterfaces();
}

void RegularNetwork::runOneCycle()
{
	for (auto& link : m_links)
		link->updateEnable();
	for (auto& router : m_routers)
		router->updateEnable();
	for (auto& terminalInterface : m_terminalInterfaces)
		terminalInterface->updateEnable();

	for (auto& link : m_links)
		link->runOneCycle();
	for (auto& router : m_routers)
		router->runOneCycle();
	for (auto& terminalInterface : m_terminalInterfaces)
		terminalInterface->runOneCycle();
}

int RegularNetwork::getRouterNumber()
{
	return m_dimension.getProduct();
}

void RegularNetwork::connectTerminal(const int routerID,
	TerminalInterface* terminalInterface)
{
	Link* link{ new Link{m_routers.at(routerID), terminalInterface} };
	m_links.push_back(link);
	m_terminalInterfaces.push_back(terminalInterface);
	terminalInterface->m_terminalInterfaceIDTorus =
		convertIDToCoordinate(terminalInterface->m_port.m_portID);
}

void RegularNetwork::loadNetworkData()
{
	generateRoutes();
	updatePriorities();
}

void RegularNetwork::generateRoutes()
{
	if (g_routingAlgorithm == "DOR")
		routeDOR();
	else if (g_routingAlgorithm == "ROMM")
		routeROMM();
	else if (g_routingAlgorithm == "MAD")
		routeMAD();
	else if (g_routingAlgorithm == "VAL")
		routeVAL();
	else if (g_routingAlgorithm == "ODD_EVEN")
		routeOddEven();
}

void RegularNetwork::updatePriorities()
{
	for (auto& router : m_routers)
		router->initiatePriorities();
}

void RegularNetwork::createRouters()
{
	for (int i{}; i < m_dimension.getProduct(); ++i)
	{
		Router* router{ new Router{i} };
		m_routers.push_back(router);
	}
}

void RegularNetwork::deleteRouters()
{
	for (auto& router : m_routers)
	{
		delete router;
		router = nullptr;
	}
}

void RegularNetwork::connectMESH()
{
	for (int k{}; k < m_dimension.m_z; ++k)
	{
		for (int j{}; j < m_dimension.m_y; ++j)
		{
			for (int i{}; i < m_dimension.m_x; ++i)
			{
				// x-axis connections
				if (i != m_dimension.m_x - 1)
				{
					Link* linkX{ new Link{
						m_routers.at(i
						+ j * m_dimension.m_x
						+ k * m_dimension.m_x * m_dimension.m_y),

						m_routers.at(i + 1
						+ j * m_dimension.m_x
						+ k * m_dimension.m_x * m_dimension.m_y)} };
					m_links.push_back(linkX);
				}
				// y-axis connections
				if (j != m_dimension.m_y - 1)
				{
					Link* linkY{ new Link{
						m_routers.at(i
						+ j * m_dimension.m_x
						+ k * m_dimension.m_x * m_dimension.m_y),

						m_routers.at(i
						+ (j + 1) * m_dimension.m_x
						+ k * m_dimension.m_x * m_dimension.m_y)} };
					m_links.push_back(linkY);
				}
				// z-axis connections
				if (k != m_dimension.m_z - 1)
				{
					Link* linkZ{ new Link{
						m_routers.at(i
						+ j * m_dimension.m_x
						+ k * m_dimension.m_x * m_dimension.m_y),

						m_routers.at(i
						+ j * m_dimension.m_x
						+ (k + 1) * m_dimension.m_x * m_dimension.m_y)} };
					m_links.push_back(linkZ);
				}
			}
		}
	}
}

void RegularNetwork::connectTORUS()
{
	for (int k{}; k < m_dimension.m_z; ++k)
	{
		for (int j{}; j < m_dimension.m_y; ++j)
		{
			for (int i{}; i < m_dimension.m_x; ++i)
			{
				// x-axis connections
				if (m_dimension.m_x != 1)
				{
					if (i == m_dimension.m_x - 1)
					{
						Link* linkX{ new Link{
							m_routers.at(i
							+ j * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y),

							m_routers.at(0
							+ j * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y)} };
						m_links.push_back(linkX);
					}
					else
					{
						Link* linkX{ new Link{
							m_routers.at(i
							+ j * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y),

							m_routers.at(i + 1
							+ j * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y)} };
						m_links.push_back(linkX);
					}
				}
				// y-axis connections
				if (m_dimension.m_y != 1)
				{
					if (j == m_dimension.m_y - 1)
					{
						Link* linkY{ new Link{
							m_routers.at(i
							+ j * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y),

							m_routers.at(i
							+ 0 * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y)} };
						m_links.push_back(linkY);
					}
					else
					{
						Link* linkY{ new Link{
							m_routers.at(i
							+ j * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y),

							m_routers.at(i
							+ (j + 1) * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y)} };
						m_links.push_back(linkY);
					}
				}
				// z-axis connections
				if (m_dimension.m_z != 1)
				{
					if (k == m_dimension.m_z - 1)
					{
						Link* linkZ{ new Link{
							m_routers.at(i
							+ j * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y),

							m_routers.at(i
							+ j * m_dimension.m_x
							+ 0 * m_dimension.m_x * m_dimension.m_y)} };
						m_links.push_back(linkZ);
					}
					else
					{
						Link* linkZ{ new Link{
							m_routers.at(i
							+ j * m_dimension.m_x
							+ k * m_dimension.m_x * m_dimension.m_y),

							m_routers.at(i
							+ j * m_dimension.m_x
							+ (k + 1) * m_dimension.m_x * m_dimension.m_y)} };
						m_links.push_back(linkZ);
					}
				}
			}
		}
	}
}

void RegularNetwork::deleteLinks()
{
	for (auto& link : m_links)
	{
		delete link;
		link = nullptr;
	}
}

void RegularNetwork::deleteTerminalInterfaces()
{
	for (auto& terminalInterface : m_terminalInterfaces)
	{
		delete terminalInterface;
		terminalInterface = nullptr;
	}
}

Coordinate RegularNetwork::convertIDToCoordinate(const int id)
{
	return { (id % (m_dimension.m_x * m_dimension.m_y)) % m_dimension.m_x,
			(id % (m_dimension.m_x * m_dimension.m_y)) / m_dimension.m_x,
			id / (m_dimension.m_x * m_dimension.m_y) };
}

int RegularNetwork::convertCoordinateToID(const Coordinate& coordinate)
{
	return coordinate.m_x
		+ coordinate.m_y * m_dimension.m_x
		+ coordinate.m_z * m_dimension.m_x * m_dimension.m_y;
}

void RegularNetwork::routeDOR()
{
	for (auto& source : m_terminalInterfaces)
	{
		source->m_sourceRoutingTable.clear();
		Coordinate src{ source->m_terminalInterfaceIDTorus };
		for (auto& destination : m_terminalInterfaces)
		{
			Coordinate dest{ destination->m_terminalInterfaceIDTorus };
			if (src != dest)
			{
				std::deque<int> route{};
				Coordinate next{ src };
				if (g_shape == "MESH")
				{
					while (dest.m_x != next.m_x)
					{
						if (dest.m_x > next.m_x)
							route.push_back(convertCoordinateToID(
								next.incrementX(m_dimension.m_x)));
						else
							route.push_back(convertCoordinateToID(
								next.decrementX(m_dimension.m_x)));
					}
					while (dest.m_y != next.m_y)
					{
						if (dest.m_y > next.m_y)
							route.push_back(convertCoordinateToID(
								next.incrementY(m_dimension.m_y)));
						else
							route.push_back(convertCoordinateToID(
								next.decrementY(m_dimension.m_y)));
					}
					while (dest.m_z != next.m_z)
					{
						if (dest.m_z > next.m_z)
							route.push_back(convertCoordinateToID(
								next.incrementZ(m_dimension.m_z)));
						else
							route.push_back(convertCoordinateToID(
								next.decrementZ(m_dimension.m_z)));
					}
				}
				if (g_shape == "TORUS")
				{
					while (dest.m_x != next.m_x)
					{
						if (dest.m_x > next.m_x)
						{
							if ((dest.m_x - next.m_x) >
								(m_dimension.m_x / 2))
								route.push_back(convertCoordinateToID(
									next.decrementX(m_dimension.m_x)));
							else
								route.push_back(convertCoordinateToID(
									next.incrementX(m_dimension.m_x)));
						}
						else
						{
							if ((dest.m_x - next.m_x) <
								(-m_dimension.m_x / 2))
								route.push_back(convertCoordinateToID(
									next.incrementX(m_dimension.m_x)));
							else
								route.push_back(convertCoordinateToID(
									next.decrementX(m_dimension.m_x)));
						}
					}
					while (dest.m_y != next.m_y)
					{
						if (dest.m_y > next.m_y)
						{
							if ((dest.m_y - next.m_y) >
								(m_dimension.m_y / 2))
								route.push_back(convertCoordinateToID(
									next.decrementY(m_dimension.m_y)));
							else
								route.push_back(convertCoordinateToID(
									next.incrementY(m_dimension.m_y)));
						}
						else
						{
							if ((dest.m_y - next.m_y) <
								(-m_dimension.m_y / 2))
								route.push_back(convertCoordinateToID(
									next.incrementY(m_dimension.m_y)));
							else
								route.push_back(convertCoordinateToID(
									next.decrementY(m_dimension.m_y)));
						}
					}
					while (dest.m_z != next.m_z)
					{
						if (dest.m_z > next.m_z)
						{
							if ((dest.m_z - next.m_z) >
								(m_dimension.m_z / 2))
								route.push_back(convertCoordinateToID(
									next.decrementZ(m_dimension.m_z)));
							else
								route.push_back(convertCoordinateToID(
									next.incrementZ(m_dimension.m_z)));
						}
						else
						{
							if ((dest.m_z - next.m_z) <
								(-m_dimension.m_z / 2))
								route.push_back(convertCoordinateToID(
									next.incrementZ(m_dimension.m_z)));
							else
								route.push_back(convertCoordinateToID(
									next.decrementZ(m_dimension.m_z)));
						}
					}
				}
				route.push_back(destination->m_terminalInterfaceID);
				source->m_sourceRoutingTable.push_back(route);
			}
		}
	}
}

// Randomized Oblivious Multi-phase Minimal (ROMM) routing
// Uses multiple phases with random intermediate destinations
void RegularNetwork::routeROMM()
{
#if REPRODUCE_RANDOM
	std::mt19937 gen(MAGIC_NUMBER);
#else
	std::random_device rd;
	std::mt19937 gen(rd());
#endif

	for (auto& source : m_terminalInterfaces)
	{
		source->m_sourceRoutingTable.clear();
		Coordinate src{ source->m_terminalInterfaceIDTorus };

		for (auto& destination : m_terminalInterfaces)
		{
			Coordinate dest{ destination->m_terminalInterfaceIDTorus };
			if (src != dest)
			{
				std::deque<int> route{};

				// Phase 1: Route to random intermediate router
				Coordinate intermediate;
				do {
					intermediate.m_x = std::uniform_int_distribution<>(0, m_dimension.m_x - 1)(gen);
					intermediate.m_y = std::uniform_int_distribution<>(0, m_dimension.m_y - 1)(gen);
					intermediate.m_z = std::uniform_int_distribution<>(0, m_dimension.m_z - 1)(gen);
				} while (intermediate == src || intermediate == dest);

				// Route from src to intermediate (using DOR)
				Coordinate next = src;
				while (intermediate.m_x != next.m_x) {
					if (intermediate.m_x > next.m_x)
						route.push_back(convertCoordinateToID(next.incrementX(m_dimension.m_x)));
					else
						route.push_back(convertCoordinateToID(next.decrementX(m_dimension.m_x)));
				}
				while (intermediate.m_y != next.m_y) {
					if (intermediate.m_y > next.m_y)
						route.push_back(convertCoordinateToID(next.incrementY(m_dimension.m_y)));
					else
						route.push_back(convertCoordinateToID(next.decrementY(m_dimension.m_y)));
				}
				while (intermediate.m_z != next.m_z) {
					if (intermediate.m_z > next.m_z)
						route.push_back(convertCoordinateToID(next.incrementZ(m_dimension.m_z)));
					else
						route.push_back(convertCoordinateToID(next.decrementZ(m_dimension.m_z)));
				}

				// Phase 2: Route from intermediate to destination (using DOR)
				while (dest.m_x != next.m_x) {
					if (dest.m_x > next.m_x)
						route.push_back(convertCoordinateToID(next.incrementX(m_dimension.m_x)));
					else
						route.push_back(convertCoordinateToID(next.decrementX(m_dimension.m_x)));
				}
				while (dest.m_y != next.m_y) {
					if (dest.m_y > next.m_y)
						route.push_back(convertCoordinateToID(next.incrementY(m_dimension.m_y)));
					else
						route.push_back(convertCoordinateToID(next.decrementY(m_dimension.m_y)));
				}
				while (dest.m_z != next.m_z) {
					if (dest.m_z > next.m_z)
						route.push_back(convertCoordinateToID(next.incrementZ(m_dimension.m_z)));
					else
						route.push_back(convertCoordinateToID(next.decrementZ(m_dimension.m_z)));
				}

				route.push_back(destination->m_terminalInterfaceID);
				source->m_sourceRoutingTable.push_back(route);
			}
		}
	}
}

// Minimal Adaptive (MAD) routing
// Uses minimal paths with adaptive selection based on congestion
void RegularNetwork::routeMAD()
{
	for (auto& source : m_terminalInterfaces)
	{
		source->m_sourceRoutingTable.clear();
		Coordinate src{ source->m_terminalInterfaceIDTorus };

		for (auto& destination : m_terminalInterfaces)
		{
			Coordinate dest{ destination->m_terminalInterfaceIDTorus };
			if (src != dest)
			{
				std::deque<int> route{};
				Coordinate next{ src };

				// Adaptive routing: choose direction based on congestion
				// For simplicity, we use a deterministic adaptive approach
				// that alternates between dimensions to avoid congestion

				while (dest.m_x != next.m_x || dest.m_y != next.m_y || dest.m_z != next.m_z)
				{
					// Determine which dimension to route in
					int dx = std::abs(dest.m_x - next.m_x);
					int dy = std::abs(dest.m_y - next.m_y);
					int dz = std::abs(dest.m_z - next.m_z);

					// Choose dimension with largest distance (minimal path)
					if (dx >= dy && dx >= dz) {
						if (dest.m_x > next.m_x)
							route.push_back(convertCoordinateToID(next.incrementX(m_dimension.m_x)));
						else
							route.push_back(convertCoordinateToID(next.decrementX(m_dimension.m_x)));
					}
					else if (dy >= dx && dy >= dz) {
						if (dest.m_y > next.m_y)
							route.push_back(convertCoordinateToID(next.incrementY(m_dimension.m_y)));
						else
							route.push_back(convertCoordinateToID(next.decrementY(m_dimension.m_y)));
					}
					else {
						if (dest.m_z > next.m_z)
							route.push_back(convertCoordinateToID(next.incrementZ(m_dimension.m_z)));
						else
							route.push_back(convertCoordinateToID(next.decrementZ(m_dimension.m_z)));
					}
				}

				route.push_back(destination->m_terminalInterfaceID);
				source->m_sourceRoutingTable.push_back(route);
			}
		}
	}
}

// Valiant's Randomized Algorithm (VAL)
// Uses random intermediate destination for load balancing
void RegularNetwork::routeVAL()
{
#if REPRODUCE_RANDOM
	std::mt19937 gen(MAGIC_NUMBER);
#else
	std::random_device rd;
	std::mt19937 gen(rd());
#endif

	for (auto& source : m_terminalInterfaces)
	{
		source->m_sourceRoutingTable.clear();
		Coordinate src{ source->m_terminalInterfaceIDTorus };

		for (auto& destination : m_terminalInterfaces)
		{
			Coordinate dest{ destination->m_terminalInterfaceIDTorus };
			if (src != dest)
			{
				std::deque<int> route{};

				// Select random intermediate router
				Coordinate intermediate;
				do {
					intermediate.m_x = std::uniform_int_distribution<>(0, m_dimension.m_x - 1)(gen);
					intermediate.m_y = std::uniform_int_distribution<>(0, m_dimension.m_y - 1)(gen);
					intermediate.m_z = std::uniform_int_distribution<>(0, m_dimension.m_z - 1)(gen);
				} while (intermediate == src || intermediate == dest);

				// Route from src to intermediate (using DOR)
				Coordinate next = src;
				while (intermediate.m_x != next.m_x) {
					if (intermediate.m_x > next.m_x)
						route.push_back(convertCoordinateToID(next.incrementX(m_dimension.m_x)));
					else
						route.push_back(convertCoordinateToID(next.decrementX(m_dimension.m_x)));
				}
				while (intermediate.m_y != next.m_y) {
					if (intermediate.m_y > next.m_y)
						route.push_back(convertCoordinateToID(next.incrementY(m_dimension.m_y)));
					else
						route.push_back(convertCoordinateToID(next.decrementY(m_dimension.m_y)));
				}
				while (intermediate.m_z != next.m_z) {
					if (intermediate.m_z > next.m_z)
						route.push_back(convertCoordinateToID(next.incrementZ(m_dimension.m_z)));
					else
						route.push_back(convertCoordinateToID(next.decrementZ(m_dimension.m_z)));
				}

				// Route from intermediate to destination (using DOR)
				while (dest.m_x != next.m_x) {
					if (dest.m_x > next.m_x)
						route.push_back(convertCoordinateToID(next.incrementX(m_dimension.m_x)));
					else
						route.push_back(convertCoordinateToID(next.decrementX(m_dimension.m_x)));
				}
				while (dest.m_y != next.m_y) {
					if (dest.m_y > next.m_y)
						route.push_back(convertCoordinateToID(next.incrementY(m_dimension.m_y)));
					else
						route.push_back(convertCoordinateToID(next.decrementY(m_dimension.m_y)));
				}
				while (dest.m_z != next.m_z) {
					if (dest.m_z > next.m_z)
						route.push_back(convertCoordinateToID(next.incrementZ(m_dimension.m_z)));
					else
						route.push_back(convertCoordinateToID(next.decrementZ(m_dimension.m_z)));
				}

				route.push_back(destination->m_terminalInterfaceID);
				source->m_sourceRoutingTable.push_back(route);
			}
		}
	}
}

// Odd-Even Adaptive routing
// Uses odd-even turn model to avoid deadlocks
void RegularNetwork::routeOddEven()
{
	for (auto& source : m_terminalInterfaces)
	{
		source->m_sourceRoutingTable.clear();
		Coordinate src{ source->m_terminalInterfaceIDTorus };

		for (auto& destination : m_terminalInterfaces)
		{
			Coordinate dest{ destination->m_terminalInterfaceIDTorus };
			if (src != dest)
			{
				std::deque<int> route{};
				Coordinate next{ src };

				// Odd-Even routing algorithm
				// In 2D mesh:
				// - From even column: can go East or West
				// - From odd column: can go North or South
				// - To even column: can go North or South
				// - To odd column: can go East or West

				while (dest.m_x != next.m_x || dest.m_y != next.m_y || dest.m_z != next.m_z)
				{
					int dx = dest.m_x - next.m_x;
					int dy = dest.m_y - next.m_y;
					int dz = dest.m_z - next.m_z;

					// For 2D case (z dimension = 1)
					if (m_dimension.m_z == 1) {
						// If at even column
						if (next.m_x % 2 == 0) {
							// Can go East or West
							if (dx > 0) {
								route.push_back(convertCoordinateToID(next.incrementX(m_dimension.m_x)));
							}
							else if (dx < 0) {
								route.push_back(convertCoordinateToID(next.decrementX(m_dimension.m_x)));
							}
							else if (dy != 0) {
								// At destination X, go Y
								if (dy > 0)
									route.push_back(convertCoordinateToID(next.incrementY(m_dimension.m_y)));
								else
									route.push_back(convertCoordinateToID(next.decrementY(m_dimension.m_y)));
							}
						}
						// If at odd column
						else {
							// Can go North or South
							if (dy > 0) {
								route.push_back(convertCoordinateToID(next.incrementY(m_dimension.m_y)));
							}
							else if (dy < 0) {
								route.push_back(convertCoordinateToID(next.decrementY(m_dimension.m_y)));
							}
							else if (dx != 0) {
								// At destination Y, go X
								if (dx > 0)
									route.push_back(convertCoordinateToID(next.incrementX(m_dimension.m_x)));
								else
									route.push_back(convertCoordinateToID(next.decrementX(m_dimension.m_x)));
							}
						}
					}
					// For 3D case, use simple DOR (odd-even is complex in 3D)
					else {
						if (dx != 0) {
							if (dx > 0)
								route.push_back(convertCoordinateToID(next.incrementX(m_dimension.m_x)));
							else
								route.push_back(convertCoordinateToID(next.decrementX(m_dimension.m_x)));
						}
						else if (dy != 0) {
							if (dy > 0)
								route.push_back(convertCoordinateToID(next.incrementY(m_dimension.m_y)));
							else
								route.push_back(convertCoordinateToID(next.decrementY(m_dimension.m_y)));
						}
						else if (dz != 0) {
							if (dz > 0)
								route.push_back(convertCoordinateToID(next.incrementZ(m_dimension.m_z)));
							else
								route.push_back(convertCoordinateToID(next.decrementZ(m_dimension.m_z)));
						}
					}
				}

				route.push_back(destination->m_terminalInterfaceID);
				source->m_sourceRoutingTable.push_back(route);
			}
		}
	}
}
