#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <random>
#include <chrono>
#include "Parameters.h"

enum class FlitType
{
	H,
	B,
	T
};

std::ostream& operator<<(std::ostream& stream,
	const FlitType& flitType);

struct Flit
{
	Flit(const int source,
		const std::deque<int>& route);
	Flit(const std::vector<float>& flitData,
		const int flitNumberB);
	Flit(const int packetID);
	bool operator==(const Flit& flit) const; // partially comparison; only for reorder buffer

	FlitType m_flitType{};
	int m_flitVirtualChannel{ -1 };
	int m_source{ -1 };
	std::deque<int> m_route{};
	std::vector<float> m_flitData{ std::vector<float>(g_flitSize) };
	int m_flitNumberB{ -1 };
	int m_packetID{ -1 };
};

std::ostream& operator<<(std::ostream& stream, const Flit& flit);

struct Packet
{
	Packet() = default;
	Packet(const int packetID,
		const int source,
		const int destination,
		const std::vector<float> data);

	int m_packetID{}, m_source{}, m_destination{};
	std::vector<float> m_data{};
};

std::ostream& operator<<(std::ostream& stream,
	const Packet& packet);

struct Credit
{
	Credit(const int creditVirtualChannel, const bool isTail);

	int m_creditVirtualChannel{ -1 };
	bool m_isTail{};
};

enum class VirtualChannelState
{
	I, // Idle, virtual channel is empty; input & output
	R, // Routing; input
	V, // Virtual channel allocation; input
	A, // Active, virtual channel is active; input & output
	C, // Waiting for Credits; output
	F // Waiting for Flits; input
};

std::ostream& operator<<(std::ostream& stream,
	const VirtualChannelState& virtualChannelState);

struct ControlField
{
	ControlField() = default;

	// enable
	bool m_enable{ true }; // enable signal to provide flip-flop behavior

	// input 
	VirtualChannelState m_virtualChannelState{
		VirtualChannelState::I };
	int m_routedOutputPort{ -1 }; // initial value is the router ID that it resides in
	int m_allocatedVirtualChannel{ -1 };

	// output
	VirtualChannelState m_downstreamVirtualChannelState{
		VirtualChannelState::I };
	int m_credit{ g_bufferSize }; // downstream virtual channel available buffer size
};

struct PriorityTableEntry
{
	PriorityTableEntry() = default;
	PriorityTableEntry(const int portIndex,
		const int virtualChannelIndex);
	bool operator==(const PriorityTableEntry& entry) const;

	int m_portIndex{};
	int m_virtualChannelIndex{};
};

struct Connection
{
	Connection() = default;
	Connection(const int inputPortIndex,
		const int inputVirtualChannelIndex,
		const int outputPortIndex,
		const int outputVirtualChannelIndex);

	int m_inputPortIndex{};
	int m_inputVirtualChannelIndex{};
	int m_outputPortIndex{};
	int m_outputVirtualChannelIndex{};
};

//enum class Topology
//{
//	MESH,
//	TORUS
//};

struct Coordinate
{
	Coordinate() = default;
	Coordinate(const int x, const int y, const int z);
	bool operator==(const Coordinate& coordinate) const;
	Coordinate operator-(const Coordinate& coordinate) const;
	int getProduct();
	int getSum();
	Coordinate getCoordinate();
	Coordinate incrementX(const int xLimit);
	Coordinate incrementY(const int yLimit);
	Coordinate incrementZ(const int zLimit);
	Coordinate decrementX(const int xLimit);
	Coordinate decrementY(const int yLimit);
	Coordinate decrementZ(const int zLimit);

	int m_x{}, m_y{}, m_z{};
};

//enum class RoutingAlgorithm
//{
//	DOR,  // dimension-order routing
//	ROMM, // randomized, oblivious, multi-phase, minimal
//	MAD,  // minimal-adaptive
//	VAL   // Valiant's randomlized algorithm
//};
//
//enum class InjectionProcess
//{
//	Periodic,
//	Bernoulli,
//	MMP
//};
//
//enum class TrafficPattern
//{
//	Random,
//	Permutation
//};
//
//enum class PacketSizeOption
//{
//	Uniform,
//	Fixed
//};

struct TrafficData
{
	TrafficData() = default;
	TrafficData(const float receivedPacketNumber,
		const float receivedFlitNumber,
		const float sentFlitNumber,
		const float sentPacketNumber,
		const float accumulatedLatency);

	float m_receivedPacketNumber{},
		m_receivedFlitNumber{},
		m_sentPacketNumber{},
		m_sentFlitNumber{},
		m_accumulatedLatency{};
};

struct Benchmark
{
	Benchmark();
	~Benchmark();

	std::chrono::time_point<std::chrono::high_resolution_clock>
		start{}, end{};
	std::chrono::duration<float> duration{};
};

struct TrafficInformationEntry
{
	TrafficInformationEntry() = default;
	TrafficInformationEntry(const int packetID,
		const int source,
		const int destination,
		const int packetSize,
		const std::string status,
		const float sentTime,
		const float receivedTime);

	int m_packetID{};
	int m_source{};
	int m_destination{};
	int m_packetSize{};
	std::string m_status{ "V"};
	float m_sentTime{};
	float m_receivedTime{};
};