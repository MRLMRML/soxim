#include "RegularNetwork.h"

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
