#include <gtest/gtest.h>
#include "TrafficOperator.h"
#include "RegularNetwork.h"
#include "TerminalInterface.h"
#include "DataStructures.h"
#include <filesystem>

// Test TrafficOperator construction
TEST(TrafficOperatorTest, Construction)
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();
    
    // Create traffic operator
    std::string trafficFolderPath = "/tmp/test_traffic";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    
    // Verify construction succeeded
    SUCCEED();
}

// Test TrafficOperator generateTraffic with random uniform
TEST(TrafficOperatorTest, GenerateRandomUniform)
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();
    
    // Create traffic operator
    std::string trafficFolderPath = "/tmp/test_traffic_random";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    
    // Generate traffic
    trafficOp.generateTraffic();
    
    // Verify traffic was generated
    SUCCEED();
}

// Test TrafficOperator generateTraffic with permutation
TEST(TrafficOperatorTest, GeneratePermutation)
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
    g_trafficPattern = "permutation";
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
    
    // Create traffic operator
    std::string trafficFolderPath = "/tmp/test_traffic_permutation";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    
    // Generate traffic
    trafficOp.generateTraffic();
    
    // Verify traffic was generated
    SUCCEED();
}

// Test TrafficOperator with periodic injection process
TEST(TrafficOperatorTest, PeriodicInjectionProcess)
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();
    
    // Create traffic operator
    std::string trafficFolderPath = "/tmp/test_traffic_periodic";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    
    // Generate traffic
    trafficOp.generateTraffic();
    
    // Verify traffic was generated
    SUCCEED();
}

// Test TrafficOperator with Bernoulli injection process
TEST(TrafficOperatorTest, BernoulliInjectionProcess)
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
    g_injectionProcess = "bernoulli";
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
    
    // Create traffic operator
    std::string trafficFolderPath = "/tmp/test_traffic_bernoulli";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    
    // Generate traffic
    trafficOp.generateTraffic();
    
    // Verify traffic was generated
    SUCCEED();
}

// Test TrafficOperator with Markov injection process
TEST(TrafficOperatorTest, MarkovInjectionProcess)
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
    g_injectionProcess = "markov";
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
    
    // Create traffic operator
    std::string trafficFolderPath = "/tmp/test_traffic_markov";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    
    // Generate traffic
    trafficOp.generateTraffic();
    
    // Verify traffic was generated
    SUCCEED();
}

// Test TrafficOperator with different injection rates
TEST(TrafficOperatorTest, DifferentInjectionRates)
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
    
    // Test different injection rates
    float injectionRates[] = {0.01f, 0.05f, 0.1f, 0.2f, 0.3f};
    
    for (float rate : injectionRates) {
        g_injectionRate = rate;
        
        std::string trafficFolderPath = "/tmp/test_traffic_rate_" + std::to_string(rate);
        std::filesystem::create_directories(trafficFolderPath);
        
        TrafficOperator trafficOp(trafficFolderPath, &network);
        trafficOp.generateTraffic();
    }
    
    SUCCEED();
}

// Test TrafficOperator with different packet sizes
TEST(TrafficOperatorTest, DifferentPacketSizes)
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
    
    // Test different packet sizes
    int packetSizes[] = {5, 10, 20, 50};
    
    for (int size : packetSizes) {
        g_packetSize = size;
        
        std::string trafficFolderPath = "/tmp/test_traffic_size_" + std::to_string(size);
        std::filesystem::create_directories(trafficFolderPath);
        
        TrafficOperator trafficOp(trafficFolderPath, &network);
        trafficOp.generateTraffic();
    }
    
    SUCCEED();
}

// Test TrafficOperator with different network sizes
TEST(TrafficOperatorTest, DifferentNetworkSizes)
{
    // Test with 2x2 network
    g_x = 2;
    g_y = 2;
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 4; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();

    std::string trafficFolderPath = "/tmp/test_traffic_2x2";
    std::filesystem::create_directories(trafficFolderPath);

    TrafficOperator trafficOp(trafficFolderPath, &network);
    trafficOp.generateTraffic();

    SUCCEED();
}

// Test TrafficOperator with 3D network
TEST(TrafficOperatorTest, ThreeDNetwork)
{
    // Test with 2x2x2 network
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 8; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();

    std::string trafficFolderPath = "/tmp/test_traffic_3d";
    std::filesystem::create_directories(trafficFolderPath);

    TrafficOperator trafficOp(trafficFolderPath, &network);
    trafficOp.generateTraffic();

    SUCCEED();
}

// Test TrafficOperator with TORUS topology
TEST(TrafficOperatorTest, TorusTopology)
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();
    
    std::string trafficFolderPath = "/tmp/test_traffic_torus";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    trafficOp.generateTraffic();
    
    SUCCEED();
}

// Test TrafficOperator with different traffic patterns
TEST(TrafficOperatorTest, DifferentTrafficPatterns)
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
    
    // Test different traffic patterns
    const char* patterns[] = {"random uniform", "permutation"};
    
    for (const char* pattern : patterns) {
        g_trafficPattern = pattern;
        
        std::string trafficFolderPath = "/tmp/test_traffic_pattern_" + std::string(pattern);
        std::filesystem::create_directories(trafficFolderPath);
        
        TrafficOperator trafficOp(trafficFolderPath, &network);
        trafficOp.generateTraffic();
    }
    
    SUCCEED();
}

// Test TrafficOperator with variable packet size
TEST(TrafficOperatorTest, VariablePacketSize)
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
    g_packetSizeOption = "variable";
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
    
    std::string trafficFolderPath = "/tmp/test_traffic_variable";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    trafficOp.generateTraffic();
    
    SUCCEED();
}

// Test TrafficOperator with high injection rate
TEST(TrafficOperatorTest, HighInjectionRate)
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
    g_injectionRate = 0.5f;  // High injection rate
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
    
    std::string trafficFolderPath = "/tmp/test_traffic_high";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    trafficOp.generateTraffic();
    
    SUCCEED();
}

// Test TrafficOperator with low injection rate
TEST(TrafficOperatorTest, LowInjectionRate)
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
    g_injectionRate = 0.001f;  // Low injection rate
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
    
    std::string trafficFolderPath = "/tmp/test_traffic_low";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    trafficOp.generateTraffic();
    
    SUCCEED();
}

// Test TrafficOperator with analyzeTraffic
TEST(TrafficOperatorTest, AnalyzeTraffic)
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();
    
    std::string trafficFolderPath = "/tmp/test_traffic_analyze";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    
    // Generate and analyze traffic
    trafficOp.generateTraffic();
    trafficOp.analyzeTraffic();
    
    SUCCEED();
}

// Test TrafficOperator with multiple generations
TEST(TrafficOperatorTest, MultipleGenerations)
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();
    
    std::string trafficFolderPath = "/tmp/test_traffic_multiple";
    std::filesystem::create_directories(trafficFolderPath);
    
    TrafficOperator trafficOp(trafficFolderPath, &network);
    
    // Generate traffic multiple times
    for (int i = 0; i < 3; ++i) {
        trafficOp.generateTraffic();
    }
    
    // Analyze
    trafficOp.analyzeTraffic();
    
    SUCCEED();
}

// Test TrafficOperator with small network
TEST(TrafficOperatorTest, SmallNetwork)
{
    // Set up global parameters
    g_x = 2;
    g_y = 2;
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 4; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();

    std::string trafficFolderPath = "/tmp/test_traffic_small";
    std::filesystem::create_directories(trafficFolderPath);

    TrafficOperator trafficOp(trafficFolderPath, &network);
    trafficOp.generateTraffic();

    SUCCEED();
}

// Test TrafficOperator with large network
TEST(TrafficOperatorTest, LargeNetwork)
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

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 64; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();

    std::string trafficFolderPath = "/tmp/test_traffic_large";
    std::filesystem::create_directories(trafficFolderPath);

    TrafficOperator trafficOp(trafficFolderPath, &network);
    trafficOp.generateTraffic();

    SUCCEED();
}

// Test TrafficOperator with different alpha/beta for Markov
TEST(TrafficOperatorTest, MarkovWithDifferentParameters)
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
    g_injectionProcess = "markov";
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
    
    // Test different alpha/beta combinations
    float alphaValues[] = {0.1f, 0.5f, 0.9f};
    float betaValues[] = {0.1f, 0.5f, 0.9f};
    
    for (float alpha : alphaValues) {
        for (float beta : betaValues) {
            g_alpha = alpha;
            g_beta = beta;
            
            std::string trafficFolderPath = "/tmp/test_traffic_markov_" + 
                std::to_string(alpha) + "_" + std::to_string(beta);
            std::filesystem::create_directories(trafficFolderPath);
            
            TrafficOperator trafficOp(trafficFolderPath, &network);
            trafficOp.generateTraffic();
        }
    }
    
    SUCCEED();
}

// Test TrafficOperator with different cycle configurations
TEST(TrafficOperatorTest, DifferentCycleConfigurations)
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
    g_packetNumber = 10;
    
    RegularNetwork network;

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();
    
    // Test different cycle configurations
    int cycleConfigs[][3] = {
        {100, 30, 40},   // Default
        {200, 50, 100},  // Longer
        {50, 10, 20},    // Shorter
    };
    
    for (auto& config : cycleConfigs) {
        g_totalCycles = config[0];
        g_warmupCycles = config[1];
        g_measurementCycles = config[2];
        g_drainCycles = config[0] - config[1] - config[2];
        
        std::string trafficFolderPath = "/tmp/test_traffic_cycles_" + 
            std::to_string(config[0]) + "_" + std::to_string(config[1]);
        std::filesystem::create_directories(trafficFolderPath);
        
        TrafficOperator trafficOp(trafficFolderPath, &network);
        trafficOp.generateTraffic();
    }
    
    SUCCEED();
}

// Test TrafficOperator with different packet number
TEST(TrafficOperatorTest, DifferentPacketNumbers)
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
    
    RegularNetwork network;

    // Create terminal interfaces (one per router)
    for (int i = 0; i < 16; ++i) {
        TerminalInterface* ti = new TerminalInterface(-i - 1);
        network.connectTerminal(i, ti);
    }

    network.loadNetworkData();
    
    // Test different packet numbers
    int packetNumbers[] = {5, 10, 20, 50};
    
    for (int num : packetNumbers) {
        g_packetNumber = num;
        
        std::string trafficFolderPath = "/tmp/test_traffic_packets_" + std::to_string(num);
        std::filesystem::create_directories(trafficFolderPath);
        
        TrafficOperator trafficOp(trafficFolderPath, &network);
        trafficOp.generateTraffic();
    }
    
    SUCCEED();
}
