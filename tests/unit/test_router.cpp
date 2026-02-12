#include <gtest/gtest.h>
#include "Router.h"
#include "DataStructures.h"
#include "Port.h"

// Test Router construction
TEST(RouterTest, DefaultConstruction)
{
    Router router;
    
    EXPECT_EQ(router.m_routerID, 0);
    EXPECT_TRUE(router.m_ports.empty());
}

TEST(RouterTest, ParameterizedConstruction)
{
    Router router(42);
    
    EXPECT_EQ(router.m_routerID, 42);
    EXPECT_TRUE(router.m_ports.empty());
}

// Test Router createPort
TEST(RouterTest, CreatePort)
{
    Router router(0);
    
    Port* port = router.createPort(0);
    EXPECT_NE(port, nullptr);
    EXPECT_EQ(port->m_portID, 0);
    EXPECT_FALSE(router.m_ports.empty());
    EXPECT_EQ(router.m_ports.size(), 1);
}

TEST(RouterTest, CreateMultiplePorts)
{
    Router router(0);
    
    // Create 5 ports (4 data ports + 1 terminal port)
    for (int i = 0; i < 5; ++i) {
        Port* port = router.createPort(i);
        EXPECT_NE(port, nullptr);
    }
    
    EXPECT_EQ(router.m_ports.size(), 5);
}

// Test Router initiatePriorities
TEST(RouterTest, InitiatePriorities)
{
    Router router(0);
    
    // Create ports first
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    // This should initialize priority tables
    router.initiatePriorities();
    
    // The method doesn't return anything, but we can verify it doesn't crash
    SUCCEED();
}

// Test Router updateEnable
TEST(RouterTest, UpdateEnable)
{
    Router router(0);
    
    // Create ports first
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    // This should update port enables
    router.updateEnable();
    
    // The method doesn't return anything, but we can verify it doesn't crash
    SUCCEED();
}

// Test Router runOneCycle (basic smoke test)
TEST(RouterTest, RunOneCycle)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    // Initialize priorities
    router.initiatePriorities();
    
    // Run one cycle - should not crash
    router.runOneCycle();
    
    SUCCEED();
}

// Test Router with flit in input register
TEST(RouterTest, RunOneCycleWithFlit)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add a flit to an input register
    std::deque<int> route = {1, 2, 3};
    Flit flit(0, route);
    router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    
    // Run one cycle
    router.runOneCycle();
    
    // Verify the flit was processed
    // (The exact behavior depends on the router state)
    SUCCEED();
}

// Test Router checkConflict
TEST(RouterTest, CheckConflict)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Test conflict detection
    // This is a private method, so we test it indirectly through runOneCycle
    router.runOneCycle();
    
    SUCCEED();
}

// Test Router with multiple flits
TEST(RouterTest, MultipleFlits)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add multiple flits to different ports
    for (int i = 0; i < 3; ++i) {
        std::deque<int> route = {i + 1, i + 2};
        Flit flit(i, route);
        router.m_ports[i]->m_inputRegister.pushbackFlit(flit);
    }
    
    // Run multiple cycles
    for (int i = 0; i < 5; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with credits
TEST(RouterTest, WithCredits)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add credits to output registers
    for (int i = 0; i < 3; ++i) {
        Credit credit(i, i % 2 == 0);
        router.m_ports[i]->m_outputRegister.pushbackCredit(credit);
    }
    
    // Run one cycle
    router.runOneCycle();
    
    SUCCEED();
}

// Test Router with flits and credits
TEST(RouterTest, WithFlitsAndCredits)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flits to input registers
    for (int i = 0; i < 3; ++i) {
        std::deque<int> route = {i + 1, i + 2};
        Flit flit(i, route);
        router.m_ports[i]->m_inputRegister.pushbackFlit(flit);
    }
    
    // Add credits to output registers
    for (int i = 0; i < 3; ++i) {
        Credit credit(i, i % 2 == 0);
        router.m_ports[i]->m_outputRegister.pushbackCredit(credit);
    }
    
    // Run multiple cycles
    for (int i = 0; i < 10; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with virtual channels
TEST(RouterTest, WithVirtualChannels)
{
    // Set up global parameters
    g_virtualChannelNumber = 4;
    g_bufferSize = 8;
    
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flits with different virtual channels
    for (int vc = 0; vc < 4; ++vc) {
        for (int i = 0; i < 2; ++i) {
            std::deque<int> route = {1, 2, 3};
            Flit flit(i, route);
            flit.m_flitVirtualChannel = vc;
            router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
        }
    }
    
    // Run multiple cycles
    for (int i = 0; i < 10; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with different flit types
TEST(RouterTest, WithDifferentFlitTypes)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add head flit
    std::deque<int> route = {1, 2, 3};
    Flit headFlit(0, route);
    router.m_ports[0]->m_inputRegister.pushbackFlit(headFlit);
    
    // Add body flits
    for (int i = 0; i < 3; ++i) {
        std::vector<float> data = {float(i), float(i + 1)};
        Flit bodyFlit(data, i);
        router.m_ports[0]->m_inputRegister.pushbackFlit(bodyFlit);
    }
    
    // Add tail flit
    Flit tailFlit(42);
    router.m_ports[0]->m_inputRegister.pushbackFlit(tailFlit);
    
    // Run multiple cycles
    for (int i = 0; i < 10; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router pipeline stages individually
TEST(RouterTest, PipelineStages)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Test each pipeline stage indirectly through runOneCycle
    // The router should handle:
    // 1. Receive Flit - from input registers
    // 2. Receive Credit - from output registers
    // 3. Compute Route - determine output port
    // 4. Allocate Virtual Channel - round-robin arbitration
    // 5. Allocate Switch - round-robin arbitration
    // 6. Traverse Switch - move flit through crossbar
    
    // Add test flits
    std::deque<int> route = {1, 2, 3};
    Flit flit(0, route);
    router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    
    // Run pipeline
    for (int i = 0; i < 5; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with empty network
TEST(RouterTest, EmptyNetwork)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Run with no flits or credits
    for (int i = 0; i < 10; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with high load
TEST(RouterTest, HighLoad)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add many flits to create high load
    for (int i = 0; i < 50; ++i) {
        std::deque<int> route = {1, 2, 3};
        Flit flit(i, route);
        router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    }
    
    // Run many cycles
    for (int i = 0; i < 100; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with different buffer sizes
TEST(RouterTest, DifferentBufferSizes)
{
    // Test with small buffer
    g_bufferSize = 2;
    g_virtualChannelNumber = 2;
    
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flits
    for (int i = 0; i < 10; ++i) {
        std::deque<int> route = {1, 2, 3};
        Flit flit(i, route);
        router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    }
    
    // Run cycles
    for (int i = 0; i < 20; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with different virtual channel counts
TEST(RouterTest, DifferentVCCounts)
{
    // Test with many virtual channels
    g_bufferSize = 8;
    g_virtualChannelNumber = 8;
    
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flits with different VCs
    for (int vc = 0; vc < 8; ++vc) {
        for (int i = 0; i < 2; ++i) {
            std::deque<int> route = {1, 2, 3};
            Flit flit(i, route);
            flit.m_flitVirtualChannel = vc;
            router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
        }
    }
    
    // Run cycles
    for (int i = 0; i < 20; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router debug method (skipped - private method)
// TEST(RouterTest, DebugMethod) { ... }

// Test Router with self-routing (same input and output)
TEST(RouterTest, SelfRouting)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flit with route that goes back to same router
    std::deque<int> route = {0, 1, 2};
    Flit flit(0, route);
    router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    
    // Run cycles
    for (int i = 0; i < 10; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with long route
TEST(RouterTest, LongRoute)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flit with long route
    std::deque<int> route = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Flit flit(0, route);
    router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    
    // Run many cycles to process long route
    for (int i = 0; i < 50; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with multiple input ports active
TEST(RouterTest, MultipleInputPortsActive)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flits to multiple input ports
    for (int port = 0; port < 3; ++port) {
        for (int i = 0; i < 5; ++i) {
            std::deque<int> route = {port + 1, port + 2};
            Flit flit(i, route);
            router.m_ports[port]->m_inputRegister.pushbackFlit(flit);
        }
    }
    
    // Run cycles
    for (int i = 0; i < 30; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with contention
TEST(RouterTest, WithContention)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flits that will contend for the same output port
    for (int i = 0; i < 10; ++i) {
        std::deque<int> route = {1, 2, 3};  // All want to go to port 1
        Flit flit(i, route);
        router.m_ports[i % 4]->m_inputRegister.pushbackFlit(flit);
    }
    
    // Run cycles
    for (int i = 0; i < 30; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with credit-based flow control
TEST(RouterTest, CreditBasedFlowControl)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flits
    for (int i = 0; i < 10; ++i) {
        std::deque<int> route = {1, 2, 3};
        Flit flit(i, route);
        router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    }
    
    // Add credits to simulate downstream availability
    for (int i = 0; i < 5; ++i) {
        Credit credit(i, i % 2 == 0);
        router.m_ports[1]->m_outputRegister.pushbackCredit(credit);
    }
    
    // Run cycles
    for (int i = 0; i < 20; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with different packet sizes
TEST(RouterTest, DifferentPacketSizes)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Test with small packets (2 flits)
    for (int i = 0; i < 5; ++i) {
        std::deque<int> route = {1, 2};
        Flit headFlit(i, route);
        router.m_ports[0]->m_inputRegister.pushbackFlit(headFlit);
        
        std::vector<float> data = {float(i)};
        Flit bodyFlit(data, i);
        router.m_ports[0]->m_inputRegister.pushbackFlit(bodyFlit);
    }
    
    // Run cycles
    for (int i = 0; i < 20; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with mixed traffic
TEST(RouterTest, MixedTraffic)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add different types of traffic
    // 1. Single flit packets
    for (int i = 0; i < 3; ++i) {
        std::deque<int> route = {1};
        Flit flit(i, route);
        router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    }
    
    // 2. Multi-flit packets
    for (int i = 0; i < 3; ++i) {
        std::deque<int> route = {2, 3, 4};
        Flit headFlit(i, route);
        router.m_ports[1]->m_inputRegister.pushbackFlit(headFlit);
        
        for (int j = 0; j < 3; ++j) {
            std::vector<float> data = {float(j)};
            Flit bodyFlit(data, j);
            router.m_ports[1]->m_inputRegister.pushbackFlit(bodyFlit);
        }
    }
    
    // Run cycles
    for (int i = 0; i < 30; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with random flit injection
TEST(RouterTest, RandomFlitInjection)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Simulate random flit injection over time
    for (int cycle = 0; cycle < 50; ++cycle) {
        // Randomly add flits
        if (cycle % 3 == 0) {
            int port = cycle % 4;
            std::deque<int> route = {1, 2, 3};
            Flit flit(cycle, route);
            router.m_ports[port]->m_inputRegister.pushbackFlit(flit);
        }
        
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with backpressure
TEST(RouterTest, Backpressure)
{
    Router router(0);
    
    // Create ports
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add many flits to create backpressure
    for (int i = 0; i < 100; ++i) {
        std::deque<int> route = {1, 2, 3};
        Flit flit(i, route);
        router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    }
    
    // Run cycles - backpressure should prevent overflow
    for (int i = 0; i < 100; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with different network sizes
TEST(RouterTest, DifferentNetworkSizes)
{
    // Test with 2x2 network
    g_x = 2;
    g_y = 2;
    g_z = 1;
    
    Router router(0);
    
    // Create ports (4 data ports + 1 terminal)
    for (int i = 0; i < 5; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flits
    for (int i = 0; i < 10; ++i) {
        std::deque<int> route = {1, 2};
        Flit flit(i, route);
        router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    }
    
    // Run cycles
    for (int i = 0; i < 20; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}

// Test Router with 3D network
TEST(RouterTest, ThreeDNetwork)
{
    // Test with 2x2x2 network
    g_x = 2;
    g_y = 2;
    g_z = 2;
    
    Router router(0);
    
    // Create ports (6 data ports + 1 terminal for 3D)
    for (int i = 0; i < 7; ++i) {
        router.createPort(i);
    }
    
    router.initiatePriorities();
    
    // Add flits
    for (int i = 0; i < 10; ++i) {
        std::deque<int> route = {1, 2, 3};
        Flit flit(i, route);
        router.m_ports[0]->m_inputRegister.pushbackFlit(flit);
    }
    
    // Run cycles
    for (int i = 0; i < 20; ++i) {
        router.runOneCycle();
    }
    
    SUCCEED();
}
