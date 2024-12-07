#pragma once
#include <sys/stat.h>
#include "RegularNetwork.h"

class TrafficOperator
{
public:
	TrafficOperator(const std::string_view trafficFolderPath,
		RegularNetwork* m_network);
	void generateTraffic();
	void generateTraffic(const int destination);
	void analyzeTraffic();

private:
	void generateRandom();
	void generatePermutation(const int destination);
	//void generateCustomize();
	int uniformDistribution(const int min, const int max); // for packet size
	void updateTrafficInformation();
	void collectData();
	void calculatePerformance();

private:
	std::string m_trafficFolderPath{};
	RegularNetwork* m_network{};
	TrafficData m_trafficData{};
};