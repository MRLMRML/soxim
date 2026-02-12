#include <gtest/gtest.h>
#include "RegularNetwork.h"
#include "DataStructures.h"

// Test routing in 2D MESH
TEST(RoutingTest, Mesh2DRouting)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "DOR";
    g_virtualChannelNumber = 2;
    g_bufferSize = 8;
    g_flitSize = 4;
    g_packetSize = 10;
    g_packetSizeOption = "fixed";
    g_injectionRate = 0.1f;
    g_injectionProcess = "periodic";
    g_alpha = 0.5f;
    g_beta = 0.5f;
    g_trafficPattern = "random uniform";
    g_totalCycles = 100;
    g_warmupCycles = 30;
    g_measurementCycles = 40;
    g_drainCycles = 30;
    g_packetNumber = 10;

    RegularNetwork network;

    // Create terminal interfaces
    for (int i = 0; i < 4; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();

    // Verify network was created successfully
    EXPECT_GT(network.getRouterNumber(), 0);
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
}

// Test routing in 2D TORUS
TEST(RoutingTest, Torus2DRouting)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "TORUS";
    g_routingAlgorithm = "DOR";
    g_virtualChannelNumber = 2;
    g_bufferSize = 8;
    g_flitSize = 4;
    g_packetSize = 10;
    g_packetSizeOption = "fixed";
    g_injectionRate = 0.1f;
    g_injectionProcess = "periodic";
    g_alpha = 0.5f;
    g_beta = 0.5f;
    g_trafficPattern = "random uniform";
    g_totalCycles = 100;
    g_warmupCycles = 30;
    g_measurementCycles = 40;
    g_drainCycles = 30;
    g_packetNumber = 10;

    RegularNetwork network;

    // Create terminal interfaces
    for (int i = 0; i < 4; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();

    // Verify network was created successfully
    EXPECT_GT(network.getRouterNumber(), 0);
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
}

// Test routing in 3D MESH
TEST(RoutingTest, Mesh3DRouting)
{
    // Set up global parameters
    g_x = 2;
    g_y = 2;
    g_z = 2;
    g_shape = "MESH";
    g_routingAlgorithm = "DOR";
    g_virtualChannelNumber = 2;
    g_bufferSize = 8;
    g_flitSize = 4;
    g_packetSize = 10;
    g_packetSizeOption = "fixed";
    g_injectionRate = 0.1f;
    g_injectionProcess = "periodic";
    g_alpha = 0.5f;
    g_beta = 0.5f;
    g_trafficPattern = "random uniform";
    g_totalCycles = 100;
    g_warmupCycles = 30;
    g_measurementCycles = 40;
    g_drainCycles = 30;
    g_packetNumber = 10;

    RegularNetwork network;

    // Create terminal interfaces
    for (int i = 0; i < 2; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();

    // Verify network was created successfully
    EXPECT_GT(network.getRouterNumber(), 0);
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
}

// Test Coordinate conversion
TEST(RoutingTest, CoordinateConversion)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "DOR";
    g_virtualChannelNumber = 2;
    g_bufferSize = 8;
    g_flitSize = 4;
    g_packetSize = 10;
    g_packetSizeOption = "fixed";
    g_injectionRate = 0.1f;
    g_injectionProcess = "periodic";
    g_alpha = 0.5f;
    g_beta = 0.5f;
    g_trafficPattern = "random uniform";
    g_totalCycles = 100;
    g_warmupCycles = 30;
    g_measurementCycles = 40;
    g_drainCycles = 30;
    g_packetNumber = 10;
    
    RegularNetwork network;
    
    // Test coordinate to ID conversion
    Coordinate coord(1, 2, 0);
    int id = coord.m_x + coord.m_y * g_x + coord.m_z * g_x * g_y;
    EXPECT_EQ(id, 1 + 2 * 4 + 0 * 4 * 4);
    EXPECT_EQ(id, 9);
}

// Test self-routing (same source and destination)
TEST(RoutingTest, SelfRouting)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "DOR";
    g_virtualChannelNumber = 2;
    g_bufferSize = 8;
    g_flitSize = 4;
    g_packetSize = 10;
    g_packetSizeOption = "fixed";
    g_injectionRate = 0.1f;
    g_injectionProcess = "periodic";
    g_alpha = 0.5f;
    g_beta = 0.5f;
    g_trafficPattern = "random uniform";
    g_totalCycles = 100;
    g_warmupCycles = 30;
    g_measurementCycles = 40;
    g_drainCycles = 30;
    g_packetNumber = 10;
    
    RegularNetwork network;
    
    // Create terminal interfaces
    for (int i = 0; i < 4; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Test routing from (0,0) to (0,0) - should not create a route
    TerminalInterface* source = network.m_terminalInterfaces[0];
    
    // Find the route to itself
    std::deque<int> route;
    for (const auto& entry : source->m_sourceRoutingTable) {
        if (entry.back() == source->m_terminalInterfaceID) {
            route = entry;
            break;
        }
    }
    
    // Verify no route exists (same source and destination)
    EXPECT_TRUE(route.empty());
}

// Test Coordinate increment/decrement operations
TEST(RoutingTest, CoordinateOperations)
{
    // Test increment X
    Coordinate coord1(1, 2, 3);
    Coordinate incX = coord1.incrementX(4);
    EXPECT_EQ(incX.m_x, 2);
    EXPECT_EQ(incX.m_y, 2);
    EXPECT_EQ(incX.m_z, 3);

    // Test increment Y
    Coordinate coord2(1, 2, 3);
    Coordinate incY = coord2.incrementY(4);
    EXPECT_EQ(incY.m_x, 1);
    EXPECT_EQ(incY.m_y, 3);
    EXPECT_EQ(incY.m_z, 3);

    // Test increment Z
    Coordinate coord3(1, 2, 3);
    Coordinate incZ = coord3.incrementZ(4);
    EXPECT_EQ(incZ.m_x, 1);
    EXPECT_EQ(incZ.m_y, 2);
    EXPECT_EQ(incZ.m_z, 0);  // 3 + 1 = 4, 4 % 4 = 0

    // Test decrement X
    Coordinate coord4(1, 2, 3);
    Coordinate decX = coord4.decrementX(4);
    EXPECT_EQ(decX.m_x, 0);
    EXPECT_EQ(decX.m_y, 2);
    EXPECT_EQ(decX.m_z, 3);

    // Test decrement Y
    Coordinate coord5(1, 2, 3);
    Coordinate decY = coord5.decrementY(4);
    EXPECT_EQ(decY.m_x, 1);
    EXPECT_EQ(decY.m_y, 1);
    EXPECT_EQ(decY.m_z, 3);

    // Test decrement Z
    Coordinate coord6(1, 2, 3);
    Coordinate decZ = coord6.decrementZ(4);
    EXPECT_EQ(decZ.m_x, 1);
    EXPECT_EQ(decZ.m_y, 2);
    EXPECT_EQ(decZ.m_z, 2);
}

// Test Coordinate wrap-around in TORUS
TEST(RoutingTest, CoordinateWrapAround)
{
    // Test wrap-around X in TORUS
    Coordinate coord1(3, 3, 0);
    Coordinate wrapX = coord1.incrementX(4);
    EXPECT_EQ(wrapX.m_x, 0);

    // Test wrap-around Y in TORUS
    Coordinate coord2(3, 3, 0);
    Coordinate wrapY = coord2.incrementY(4);
    EXPECT_EQ(wrapY.m_y, 0);

    // Test wrap-around X decrement in TORUS
    Coordinate coord3(0, 3, 0);
    Coordinate wrapXDec = coord3.decrementX(4);
    EXPECT_EQ(wrapXDec.m_x, 3);

    // Test wrap-around Y decrement in TORUS
    Coordinate coord4(3, 0, 0);
    Coordinate wrapYDec = coord4.decrementY(4);
    EXPECT_EQ(wrapYDec.m_y, 3);
}
