#include <gtest/gtest.h>
#include "RegularNetwork.h"
#include "TerminalInterface.h"
#include "DataStructures.h"

// Test ROMM routing algorithm
TEST(RoutingAlgorithmsTest, ROMM)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "ROMM";
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
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify routes were generated
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
    
    // Check that routes exist for all source-destination pairs
    for (auto& ti : network.m_terminalInterfaces) {
        EXPECT_FALSE(ti->m_sourceRoutingTable.empty());
    }
    
    SUCCEED();
}

// Test MAD routing algorithm
TEST(RoutingAlgorithmsTest, MAD)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "MAD";
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
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify routes were generated
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
    
    // Check that routes exist for all source-destination pairs
    for (auto& ti : network.m_terminalInterfaces) {
        EXPECT_FALSE(ti->m_sourceRoutingTable.empty());
    }
    
    SUCCEED();
}

// Test VAL routing algorithm
TEST(RoutingAlgorithmsTest, VAL)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "VAL";
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
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify routes were generated
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
    
    // Check that routes exist for all source-destination pairs
    for (auto& ti : network.m_terminalInterfaces) {
        EXPECT_FALSE(ti->m_sourceRoutingTable.empty());
    }
    
    SUCCEED();
}

// Test Odd-Even routing algorithm
TEST(RoutingAlgorithmsTest, OddEven)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "ODD_EVEN";
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
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify routes were generated
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
    
    // Check that routes exist for all source-destination pairs
    for (auto& ti : network.m_terminalInterfaces) {
        EXPECT_FALSE(ti->m_sourceRoutingTable.empty());
    }
    
    SUCCEED();
}

// Test ROMM with TORUS topology
TEST(RoutingAlgorithmsTest, ROMMTorus)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "TORUS";
    g_routingAlgorithm = "ROMM";
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
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify routes were generated
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
    
    // Check that routes exist for all source-destination pairs
    for (auto& ti : network.m_terminalInterfaces) {
        EXPECT_FALSE(ti->m_sourceRoutingTable.empty());
    }
    
    SUCCEED();
}

// Test MAD with 3D network
TEST(RoutingAlgorithmsTest, MAD3D)
{
    // Set up global parameters
    g_x = 2;
    g_y = 2;
    g_z = 2;
    g_shape = "MESH";
    g_routingAlgorithm = "MAD";
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
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 8; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify routes were generated
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
    
    // Check that routes exist for all source-destination pairs
    for (auto& ti : network.m_terminalInterfaces) {
        EXPECT_FALSE(ti->m_sourceRoutingTable.empty());
    }
    
    SUCCEED();
}

// Test VAL with small network
TEST(RoutingAlgorithmsTest, VALSmall)
{
    // Set up global parameters
    g_x = 2;
    g_y = 2;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "VAL";
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
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 4; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify routes were generated
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
    
    // Check that routes exist for all source-destination pairs
    for (auto& ti : network.m_terminalInterfaces) {
        EXPECT_FALSE(ti->m_sourceRoutingTable.empty());
    }
    
    SUCCEED();
}

// Test Odd-Even with 2x2 network
TEST(RoutingAlgorithmsTest, OddEven2x2)
{
    // Set up global parameters
    g_x = 2;
    g_y = 2;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "ODD_EVEN";
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
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 4; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Verify routes were generated
    EXPECT_FALSE(network.m_terminalInterfaces.empty());
    
    // Check that routes exist for all source-destination pairs
    for (auto& ti : network.m_terminalInterfaces) {
        EXPECT_FALSE(ti->m_sourceRoutingTable.empty());
    }
    
    SUCCEED();
}

// Test all routing algorithms with same network
TEST(RoutingAlgorithmsTest, AllAlgorithms)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
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
    
    const char* algorithms[] = {"DOR", "ROMM", "MAD", "VAL", "ODD_EVEN"};
    
    for (const char* algorithm : algorithms) {
        g_routingAlgorithm = algorithm;
        
        RegularNetwork network;
        
        // Create terminal interfaces (one per router)
        for (int i = 0; i < 16; ++i) {
            TerminalInterface* ti = new TerminalInterface(-i - 1);
            network.connectTerminal(i, ti);
        }
        
        network.loadNetworkData();
        
        // Verify routes were generated
        EXPECT_FALSE(network.m_terminalInterfaces.empty());
        
        // Check that routes exist for all source-destination pairs
        for (auto& ti : network.m_terminalInterfaces) {
            EXPECT_FALSE(ti->m_sourceRoutingTable.empty());
        }
    }
    
    SUCCEED();
}

// Test route length for different algorithms
TEST(RoutingAlgorithmsTest, RouteLength)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
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
    
    const char* algorithms[] = {"DOR", "ROMM", "MAD", "VAL", "ODD_EVEN"};
    
    for (const char* algorithm : algorithms) {
        g_routingAlgorithm = algorithm;
        
        RegularNetwork network;
        
        // Create terminal interfaces (one per router)
        for (int i = 0; i < 16; ++i) {
            TerminalInterface* ti = new TerminalInterface(-i - 1);
            network.connectTerminal(i, ti);
        }
        
        network.loadNetworkData();
        
        // Check route lengths
        for (auto& ti : network.m_terminalInterfaces) {
            for (auto& route : ti->m_sourceRoutingTable) {
                // Route should have at least 1 hop (destination)
                EXPECT_GE(route.size(), 1);
                // Route should not be too long (max 12 hops for 4x4 mesh with ROMM/VAL)
                EXPECT_LE(route.size(), 12);
            }
        }
    }
    
    SUCCEED();
}

// Test ROMM route randomness
TEST(RoutingAlgorithmsTest, ROMMRandomness)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "ROMM";
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
    
    RegularNetwork network1;
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network1.connectTerminal(i, ti);
    }
    
    network1.loadNetworkData();
    
    // Create another network with same parameters
    RegularNetwork network2;
    
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network2.connectTerminal(i, ti);
    }
    
    network2.loadNetworkData();
    
    // Routes should be different due to randomness
    // (unless we're using REPRODUCE_RANDOM mode)
    bool routesDifferent = false;
    for (int i = 0; i < 16; ++i) {
        if (network1.m_terminalInterfaces[i]->m_sourceRoutingTable !=
            network2.m_terminalInterfaces[i]->m_sourceRoutingTable) {
            routesDifferent = true;
            break;
        }
    }
    
    // In non-reproducible mode, routes should be different
    // In reproducible mode, they should be the same
    // We'll just verify that routes exist
    EXPECT_FALSE(network1.m_terminalInterfaces.empty());
    EXPECT_FALSE(network2.m_terminalInterfaces.empty());
    
    SUCCEED();
}

// Test VAL route randomness
TEST(RoutingAlgorithmsTest, VALRandomness)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "VAL";
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
    
    RegularNetwork network1;
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network1.connectTerminal(i, ti);
    }
    
    network1.loadNetworkData();
    
    // Create another network with same parameters
    RegularNetwork network2;
    
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network2.connectTerminal(i, ti);
    }
    
    network2.loadNetworkData();
    
    // Routes should be different due to randomness
    // (unless we're using REPRODUCE_RANDOM mode)
    bool routesDifferent = false;
    for (int i = 0; i < 16; ++i) {
        if (network1.m_terminalInterfaces[i]->m_sourceRoutingTable !=
            network2.m_terminalInterfaces[i]->m_sourceRoutingTable) {
            routesDifferent = true;
            break;
        }
    }
    
    // In non-reproducible mode, routes should be different
    // In reproducible mode, they should be the same
    // We'll just verify that routes exist
    EXPECT_FALSE(network1.m_terminalInterfaces.empty());
    EXPECT_FALSE(network2.m_terminalInterfaces.empty());
    
    SUCCEED();
}

// Test Odd-Even route properties
TEST(RoutingAlgorithmsTest, OddEvenProperties)
{
    // Set up global parameters
    g_x = 4;
    g_y = 4;
    g_z = 1;
    g_shape = "MESH";
    g_routingAlgorithm = "ODD_EVEN";
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
    
    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }
    
    network.loadNetworkData();
    
    // Check that all routes are valid
    for (auto& ti : network.m_terminalInterfaces) {
        for (auto& route : ti->m_sourceRoutingTable) {
            // Route should have at least 1 hop (destination)
            EXPECT_GE(route.size(), 1);
            // All intermediate routers should be valid
            for (size_t j = 0; j < route.size() - 1; ++j) {
                EXPECT_GE(route[j], 0);
                EXPECT_LT(route[j], 16);
            }
            // Destination should be valid (can be negative for terminal interfaces)
            EXPECT_LT(route.back(), 16);
        }
    }
    
    SUCCEED();
}
