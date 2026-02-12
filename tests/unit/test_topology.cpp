#include <gtest/gtest.h>
#include "RegularNetwork.h"
#include "DataStructures.h"

// Declare friend class for accessing private members
class TopologyTest;

// Test topology creation for 2D MESH
TEST(TopologyTest, Mesh2DConstruction)
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
    
    // Verify network size
    EXPECT_EQ(network.getRouterNumber(), 16);
    EXPECT_EQ(network.m_terminalInterfaces.size(), 4);
}

// Test topology creation for 2D TORUS
TEST(TopologyTest, Torus2DConstruction)
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
    
    // Verify network size
    EXPECT_EQ(network.getRouterNumber(), 16);
    EXPECT_EQ(network.m_terminalInterfaces.size(), 4);
}

// Test topology creation for 3D MESH
TEST(TopologyTest, Mesh3DConstruction)
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
    
    // Verify network size
    EXPECT_EQ(network.getRouterNumber(), 8);
    EXPECT_EQ(network.m_terminalInterfaces.size(), 2);
}

// Test topology creation for 3D TORUS
TEST(TopologyTest, Torus3DConstruction)
{
    // Set up global parameters
    g_x = 2;
    g_y = 2;
    g_z = 2;
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
    for (int i = 0; i < 2; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify network size
    EXPECT_EQ(network.getRouterNumber(), 8);
    EXPECT_EQ(network.m_terminalInterfaces.size(), 2);
}

// Test small network (1x1)
TEST(TopologyTest, SmallNetwork1x1)
{
    // Set up global parameters
    g_x = 1;
    g_y = 1;
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
    for (int i = 0; i < 1; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify network size
    EXPECT_EQ(network.getRouterNumber(), 1);
    EXPECT_EQ(network.m_terminalInterfaces.size(), 1);
}

// Test network with different sizes
TEST(TopologyTest, DifferentNetworkSizes)
{
    // Set up global parameters
    g_x = 8;
    g_y = 8;
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
    for (int i = 0; i < 8; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify network size
    EXPECT_EQ(network.getRouterNumber(), 64);
    EXPECT_EQ(network.m_terminalInterfaces.size(), 8);
}

// Test router creation and properties
TEST(TopologyTest, RouterProperties)
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

    // Verify network has the expected number of routers
    EXPECT_EQ(network.getRouterNumber(), 16);
}

// Test link creation
TEST(TopologyTest, LinkCreation)
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

    // Verify network has routers
    EXPECT_GT(network.getRouterNumber(), 0);
}

// Test terminal interface connection
TEST(TopologyTest, TerminalInterfaceConnection)
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
    
    // Verify terminal interfaces are connected
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
    
    // Verify each terminal interface has a port
    for (const auto& ti : network.m_terminalInterfaces) {
        EXPECT_NE(ti->m_port.m_portID, -1);
    }
}

// Test coordinate to ID conversion
TEST(TopologyTest, CoordinateToIDConversion)
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
    Coordinate coord(0, 0, 0);
    int id = coord.m_x + coord.m_y * g_x + coord.m_z * g_x * g_y;
    EXPECT_EQ(id, 0);
    
    Coordinate coord2(1, 2, 0);
    id = coord2.m_x + coord2.m_y * g_x + coord2.m_z * g_x * g_y;
    EXPECT_EQ(id, 1 + 2 * 4);
    EXPECT_EQ(id, 9);
    
    Coordinate coord3(3, 3, 0);
    id = coord3.m_x + coord3.m_y * g_x + coord3.m_z * g_x * g_y;
    EXPECT_EQ(id, 3 + 3 * 4);
    EXPECT_EQ(id, 15);
}
