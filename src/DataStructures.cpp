#include "DataStructures.h"

// overloading << for std::vector
template <typename T> std::ostream& operator<<(
	std::ostream& stream, const std::vector<T>& vector)
{
	for (auto element : vector)
	{
		stream << element << " ";
	}
	return stream;
}

// overloading << for std::deque
template <typename T> std::ostream& operator<<(
	std::ostream& stream, const std::deque<T>& deque)
{
	for (auto element : deque)
	{
		stream << element << " ";
	}
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const FlitType& flitType)
{
	switch (flitType)
	{
	case FlitType::H:
		stream << "H";
		break;
	case FlitType::B:
		stream << "B";
		break;
	case FlitType::T:
		stream << "T";
		break;
	}
	return stream;
}

Flit::Flit(const int source,
	const std::deque<int>& route)
	:
	m_source{ source },
	m_route{ route }
{
	m_flitType = FlitType::H;
}

Flit::Flit(const std::vector<float>& flitData, const int flitNumberB)
	:
	m_flitData{ flitData },
	m_flitNumberB{ flitNumberB }
{
	m_flitType = FlitType::B;
}

Flit::Flit(const int packetID)
	:
	m_packetID{ packetID }
{
	m_flitType = FlitType::T;
}

bool Flit::operator==(const Flit& flit) const
{
	return m_flitType == flit.m_flitType &&
		m_flitVirtualChannel == flit.m_flitVirtualChannel &&
		m_flitNumberB == flit.m_flitNumberB;
}

std::ostream& operator<<(std::ostream& stream, const Flit& flit)
{
	stream << flit.m_flitType << "|"
		<< flit.m_flitVirtualChannel << "|"
		<< flit.m_source << "|"
		<< flit.m_route << "|"
		<< flit.m_flitData << "|"
		<< flit.m_flitNumberB << "|"
		<< flit.m_packetID;
	return stream;
}

Packet::Packet(const int packetID,
	const int source,
	const int destination,
	const std::vector<float> data)
	:
	m_packetID{ packetID },
	m_source{ source },
	m_destination{ destination },
	m_data{ data } {
}

std::ostream& operator<<(std::ostream& stream, const Packet& packet)
{
	stream << packet.m_packetID << "|"
		<< packet.m_source << "|"
		<< packet.m_destination << "|"
		<< packet.m_data;
	return stream;
}

Credit::Credit(const int creditVirtualChannel,
	const bool isTail)
	:
	m_creditVirtualChannel{ creditVirtualChannel },
	m_isTail{ isTail } {
}

std::ostream& operator<<(std::ostream& stream,
	const VirtualChannelState& virtualChannelState)
{
	switch (virtualChannelState)
	{
	case VirtualChannelState::I:
		stream << "I";
		break;
	case VirtualChannelState::R:
		stream << "R";
		break;
	case VirtualChannelState::V:
		stream << "V";
		break;
	case VirtualChannelState::A:
		stream << "A";
		break;
	case VirtualChannelState::C:
		stream << "C";
		break;
	case VirtualChannelState::F:
		stream << "F";
		break;
	}
	return stream;
}

PriorityTableEntry::PriorityTableEntry(const int portIndex,
	const int virtualChannelIndex)
	:
	m_portIndex{ portIndex },
	m_virtualChannelIndex{ virtualChannelIndex } {
}

bool PriorityTableEntry::operator==(const PriorityTableEntry& entry) const
{
	return m_portIndex == entry.m_portIndex
		&& m_virtualChannelIndex == entry.m_virtualChannelIndex;
}

Connection::Connection(const int inputPortIndex,
	const int inputVirtualChannelIndex,
	const int outputPortIndex,
	const int outputVirtualChannelIndex)
	:
	m_inputPortIndex{ inputPortIndex },
	m_inputVirtualChannelIndex{ inputVirtualChannelIndex },
	m_outputPortIndex{ outputPortIndex },
	m_outputVirtualChannelIndex{ outputVirtualChannelIndex } {
}

Coordinate::Coordinate(const int x, const int y, const int z)
	:
	m_x{ x },
	m_y{ y },
	m_z{ z } {
}

bool Coordinate::operator==(const Coordinate& coordinate) const
{
	return m_x == coordinate.m_x &&
		m_y == coordinate.m_y &&
		m_z == coordinate.m_z;
}

Coordinate Coordinate::operator-(const Coordinate& coordinate) const
{
	return { m_x - coordinate.m_x,
	m_y - coordinate.m_y,
	m_z - coordinate.m_z };
}

int Coordinate::getProduct()
{
	return m_x * m_y * m_z;
}

int Coordinate::getSum()
{
	return m_x + m_y + m_z;
}

Coordinate Coordinate::getCoordinate()
{
	return { m_x, m_y, m_z };
}

Coordinate Coordinate::incrementX(const int xLimit)
{
	if (++m_x > xLimit - 1)
		m_x %= xLimit;
	return { m_x, m_y, m_z };
}

Coordinate Coordinate::incrementY(const int yLimit)
{
	if (++m_y > yLimit - 1)
		m_y %= yLimit;
	return { m_x, m_y, m_z };
}

Coordinate Coordinate::incrementZ(const int zLimit)
{
	if (++m_z > zLimit - 1)
		m_z %= zLimit;
	return { m_x, m_y, m_z };
}

Coordinate Coordinate::decrementX(const int xLimit)
{
	if (--m_x < 0)
		m_x = xLimit - 1;
	return { m_x, m_y, m_z };
}

Coordinate Coordinate::decrementY(const int yLimit)
{
	if (--m_y < 0)
		m_y = yLimit - 1;
	return { m_x, m_y, m_z };
}

Coordinate Coordinate::decrementZ(const int zLimit)
{
	if (--m_z < 0)
		m_z = zLimit - 1;
	return { m_x, m_y, m_z };
}

TrafficData::TrafficData(const float receivedPacketNumber,
	const float receivedFlitNumber,
	const float sentFlitNumber,
	const float sentPacketNumber,
	const float accumulatedLatency)
	:
	m_receivedPacketNumber{ receivedPacketNumber },
	m_receivedFlitNumber{ receivedFlitNumber },
	m_sentPacketNumber{ sentPacketNumber },
	m_sentFlitNumber{ sentFlitNumber },
	m_accumulatedLatency{ accumulatedLatency } {
}

Benchmark::Benchmark()
{
	start = std::chrono::high_resolution_clock::now();
}

Benchmark::~Benchmark()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	std::cout << "************** Simulator benchmarking **************\n"
		<< "The simulator ran for " << duration << std::endl;
}

TrafficInformationEntry::TrafficInformationEntry(const int packetID,
	const int source,
	const int destination,
	const int packetSize,
	const std::string status,
	const float sentTime,
	const float receivedTime)
	:
	m_packetID{ packetID },
	m_source{ source },
	m_destination{ destination },
	m_packetSize{ packetSize },
	m_status{ status },
	m_sentTime{ sentTime },
	m_receivedTime{ receivedTime } {
}