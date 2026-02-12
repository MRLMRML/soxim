#include <gtest/gtest.h>
#include "DataStructures.h"

// Test Flit construction and properties
TEST(FlitTest, HeadFlitConstruction)
{
    std::deque<int> route = {1, 2, 3};
    Flit flit(0, route);
    
    EXPECT_EQ(flit.m_flitType, FlitType::H);
    EXPECT_EQ(flit.m_source, 0);
    EXPECT_EQ(flit.m_route, route);
}

TEST(FlitTest, BodyFlitConstruction)
{
    std::vector<float> data = {1.0f, 2.0f, 3.0f};
    Flit flit(data, 5);
    
    EXPECT_EQ(flit.m_flitType, FlitType::B);
    EXPECT_EQ(flit.m_flitData, data);
    EXPECT_EQ(flit.m_flitNumberB, 5);
}

TEST(FlitTest, TailFlitConstruction)
{
    Flit flit(42);
    
    EXPECT_EQ(flit.m_flitType, FlitType::T);
    EXPECT_EQ(flit.m_packetID, 42);
}

TEST(FlitTest, EqualityOperator)
{
    // Flit equality only compares m_flitType, m_flitVirtualChannel, and m_flitNumberB
    // m_source and m_route are not compared
    Flit flit1(1, {1, 2});
    Flit flit2(1, {1, 2});
    Flit flit3(2, {1, 2});

    // All three have same flitType (H), same flitVirtualChannel (-1), same flitNumberB (-1)
    EXPECT_TRUE(flit1 == flit2);
    EXPECT_TRUE(flit1 == flit3);  // They are equal based on the comparison criteria
}

// Test Packet construction
TEST(PacketTest, PacketConstruction)
{
    std::vector<float> data = {1.0f, 2.0f, 3.0f};
    Packet packet(10, -1, -2, data);
    
    EXPECT_EQ(packet.m_packetID, 10);
    EXPECT_EQ(packet.m_source, -1);
    EXPECT_EQ(packet.m_destination, -2);
    EXPECT_EQ(packet.m_data, data);
}

// Test Credit construction
TEST(CreditTest, CreditConstruction)
{
    Credit credit(3, true);
    
    EXPECT_EQ(credit.m_creditVirtualChannel, 3);
    EXPECT_TRUE(credit.m_isTail);
}

// Test VirtualChannelState enum
TEST(VirtualChannelStateTest, EnumValues)
{
    EXPECT_EQ(static_cast<int>(VirtualChannelState::I), 0);
    EXPECT_EQ(static_cast<int>(VirtualChannelState::R), 1);
    EXPECT_EQ(static_cast<int>(VirtualChannelState::V), 2);
    EXPECT_EQ(static_cast<int>(VirtualChannelState::A), 3);
    EXPECT_EQ(static_cast<int>(VirtualChannelState::C), 4);
    EXPECT_EQ(static_cast<int>(VirtualChannelState::F), 5);
}

// Test ControlField construction
TEST(ControlFieldTest, DefaultConstruction)
{
    ControlField field;

    EXPECT_TRUE(field.m_enable);
    EXPECT_EQ(field.m_virtualChannelState, VirtualChannelState::I);
    EXPECT_EQ(field.m_routedOutputPort, -1);
    EXPECT_EQ(field.m_allocatedVirtualChannel, -1);
    EXPECT_EQ(field.m_downstreamVirtualChannelState, VirtualChannelState::I);
    // m_credit is initialized to g_bufferSize (which is 0 by default)
    EXPECT_EQ(field.m_credit, 0);
}

// Test PriorityTableEntry construction and equality
TEST(PriorityTableEntryTest, Construction)
{
    PriorityTableEntry entry(2, 3);
    
    EXPECT_EQ(entry.m_portIndex, 2);
    EXPECT_EQ(entry.m_virtualChannelIndex, 3);
}

TEST(PriorityTableEntryTest, EqualityOperator)
{
    PriorityTableEntry entry1(1, 2);
    PriorityTableEntry entry2(1, 2);
    PriorityTableEntry entry3(2, 1);
    
    EXPECT_TRUE(entry1 == entry2);
    EXPECT_FALSE(entry1 == entry3);
}

// Test Connection construction
TEST(ConnectionTest, Construction)
{
    Connection conn(0, 1, 2, 3);
    
    EXPECT_EQ(conn.m_inputPortIndex, 0);
    EXPECT_EQ(conn.m_inputVirtualChannelIndex, 1);
    EXPECT_EQ(conn.m_outputPortIndex, 2);
    EXPECT_EQ(conn.m_outputVirtualChannelIndex, 3);
}

// Test Coordinate construction and operations
TEST(CoordinateTest, Construction)
{
    Coordinate coord(1, 2, 3);
    
    EXPECT_EQ(coord.m_x, 1);
    EXPECT_EQ(coord.m_y, 2);
    EXPECT_EQ(coord.m_z, 3);
}

TEST(CoordinateTest, EqualityOperator)
{
    Coordinate coord1(1, 2, 3);
    Coordinate coord2(1, 2, 3);
    Coordinate coord3(1, 2, 4);
    
    EXPECT_TRUE(coord1 == coord2);
    EXPECT_FALSE(coord1 == coord3);
}

TEST(CoordinateTest, SubtractionOperator)
{
    Coordinate coord1(5, 6, 7);
    Coordinate coord2(1, 2, 3);
    Coordinate result = coord1 - coord2;
    
    EXPECT_EQ(result.m_x, 4);
    EXPECT_EQ(result.m_y, 4);
    EXPECT_EQ(result.m_z, 4);
}

TEST(CoordinateTest, GetProduct)
{
    Coordinate coord(2, 3, 4);
    
    EXPECT_EQ(coord.getProduct(), 24);
}

TEST(CoordinateTest, GetSum)
{
    Coordinate coord(2, 3, 4);
    
    EXPECT_EQ(coord.getSum(), 9);
}

TEST(CoordinateTest, IncrementX)
{
    Coordinate coord(0, 1, 2);
    Coordinate result = coord.incrementX(3);
    
    EXPECT_EQ(result.m_x, 1);
    EXPECT_EQ(result.m_y, 1);
    EXPECT_EQ(result.m_z, 2);
}

TEST(CoordinateTest, IncrementXWrapAround)
{
    Coordinate coord(2, 1, 2);
    Coordinate result = coord.incrementX(3);
    
    EXPECT_EQ(result.m_x, 0);
}

TEST(CoordinateTest, DecrementX)
{
    Coordinate coord(1, 1, 2);
    Coordinate result = coord.decrementX(3);
    
    EXPECT_EQ(result.m_x, 0);
}

TEST(CoordinateTest, DecrementXWrapAround)
{
    Coordinate coord(0, 1, 2);
    Coordinate result = coord.decrementX(3);
    
    EXPECT_EQ(result.m_x, 2);
}

TEST(CoordinateTest, IncrementY)
{
    Coordinate coord(0, 0, 2);
    Coordinate result = coord.incrementY(3);
    
    EXPECT_EQ(result.m_y, 1);
}

TEST(CoordinateTest, DecrementY)
{
    Coordinate coord(0, 1, 2);
    Coordinate result = coord.decrementY(3);
    
    EXPECT_EQ(result.m_y, 0);
}

TEST(CoordinateTest, IncrementZ)
{
    Coordinate coord(0, 0, 0);
    Coordinate result = coord.incrementZ(3);
    
    EXPECT_EQ(result.m_z, 1);
}

TEST(CoordinateTest, DecrementZ)
{
    Coordinate coord(0, 0, 1);
    Coordinate result = coord.decrementZ(3);
    
    EXPECT_EQ(result.m_z, 0);
}

// Test TrafficData construction
TEST(TrafficDataTest, DefaultConstruction)
{
    TrafficData data;
    
    EXPECT_EQ(data.m_receivedPacketNumber, 0.0f);
    EXPECT_EQ(data.m_receivedFlitNumber, 0.0f);
    EXPECT_EQ(data.m_sentPacketNumber, 0.0f);
    EXPECT_EQ(data.m_sentFlitNumber, 0.0f);
    EXPECT_EQ(data.m_accumulatedLatency, 0.0f);
}

TEST(TrafficDataTest, ParameterizedConstruction)
{
    TrafficData data(10.0f, 200.0f, 150.0f, 8.0f, 50.0f);
    
    EXPECT_EQ(data.m_receivedPacketNumber, 10.0f);
    EXPECT_EQ(data.m_receivedFlitNumber, 200.0f);
    EXPECT_EQ(data.m_sentPacketNumber, 8.0f);
    EXPECT_EQ(data.m_sentFlitNumber, 150.0f);
    EXPECT_EQ(data.m_accumulatedLatency, 50.0f);
}

// Test TrafficInformationEntry construction
TEST(TrafficInformationEntryTest, Construction)
{
    TrafficInformationEntry entry(100, -1, -2, 20, "S", 10.5f, 25.3f);
    
    EXPECT_EQ(entry.m_packetID, 100);
    EXPECT_EQ(entry.m_source, -1);
    EXPECT_EQ(entry.m_destination, -2);
    EXPECT_EQ(entry.m_packetSize, 20);
    EXPECT_EQ(entry.m_status, "S");
    EXPECT_EQ(entry.m_sentTime, 10.5f);
    EXPECT_EQ(entry.m_receivedTime, 25.3f);
}

// Test FlitType enum output operator
TEST(FlitTypeTest, OutputStream)
{
    std::ostringstream oss;
    
    oss << FlitType::H;
    EXPECT_EQ(oss.str(), "H");
    
    oss.str("");
    oss << FlitType::B;
    EXPECT_EQ(oss.str(), "B");
    
    oss.str("");
    oss << FlitType::T;
    EXPECT_EQ(oss.str(), "T");
}

// Test VirtualChannelState enum output operator
TEST(VirtualChannelStateTest, OutputStream)
{
    std::ostringstream oss;
    
    oss << VirtualChannelState::I;
    EXPECT_EQ(oss.str(), "I");
    
    oss.str("");
    oss << VirtualChannelState::R;
    EXPECT_EQ(oss.str(), "R");
    
    oss.str("");
    oss << VirtualChannelState::V;
    EXPECT_EQ(oss.str(), "V");
    
    oss.str("");
    oss << VirtualChannelState::A;
    EXPECT_EQ(oss.str(), "A");
    
    oss.str("");
    oss << VirtualChannelState::C;
    EXPECT_EQ(oss.str(), "C");
    
    oss.str("");
    oss << VirtualChannelState::F;
    EXPECT_EQ(oss.str(), "F");
}
