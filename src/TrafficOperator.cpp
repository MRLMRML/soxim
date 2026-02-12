#include "TrafficOperator.h"

TrafficOperator::TrafficOperator(const std::string_view trafficFolderPath,
	RegularNetwork* network)
	:
	m_trafficFolderPath{ trafficFolderPath },
	m_network{ network }
{
	std::ofstream writeTrafficInformation(
		m_trafficFolderPath + "TrafficInformation.csv", std::ios::out);
	writeTrafficInformation
		<< "PacketID" << ','
		<< "Source" << ','
		<< "Destination" << ','
		<< "PacketSize" << ','
		<< "Status" << ','
		<< "SentTime" << ','
		<< "ReceivedTime" << ','
		<< std::endl;
	writeTrafficInformation.close();

	std::ofstream writeTrafficData(
		m_trafficFolderPath + "TrafficData.csv", std::ios::out);
	writeTrafficData << "Data" << ',' << std::endl;
	writeTrafficData.close();
}

void TrafficOperator::generateTraffic()
{
	if (g_trafficPattern == "random uniform")
		generateRandom();
}

void TrafficOperator::generateTraffic(const int destination)
{
	if (g_trafficPattern == "permutation")
		generatePermutation(destination);
}

void TrafficOperator::analyzeTraffic()
{
	updateTrafficInformation();
	collectData();
	calculatePerformance();
}

void TrafficOperator::generateRandom()
{
#if REPRODUCE_RANDOM
	std::mt19937 gen(MAGIC_NUMBER);  // to reproduce random numbers
#else
	std::random_device rd;
	std::mt19937 gen(rd());  // to seed mersenne twister
#endif
	std::uniform_int_distribution<> randomDestination(-m_network->getRouterNumber(), -1);
	// write append open TrafficInformation.csv
	std::ofstream writeTrafficInformation(m_trafficFolderPath +
		"TrafficInformation.csv", std::ios::app);
	// write append open TrafficData.csv
	std::ofstream writeTrafficData(m_trafficFolderPath +
		"TrafficData.csv", std::ios::app);

	for (int source{ -1 }; source >= -m_network->getRouterNumber(); --source)
	{
		for (int packetID{}; packetID < g_packetNumber; ++packetID)
		{
			// write TrafficData.csv
			int packetSize{ g_packetSize };
			if (g_packetSizeOption == "random uniform")
				packetSize = uniformDistribution(1, g_packetSize);
			for (int i{}; i < packetSize; ++i)
				writeTrafficData << static_cast<float>(i) << ',';
			writeTrafficData << std::endl;

			// write m_outputTrafficDataBuffer in each terminal interface
			std::vector<float> data{};
			for (int i{}; i < packetSize; ++i)
				data.push_back(i);
			m_network->m_terminalInterfaces.at(-source - 1)->
				m_outputTrafficDataBuffer.push_back(data);

			// write TrafficInformation.csv
			int destination{};
			do destination = randomDestination(gen);
			while (destination == source);
			writeTrafficInformation
				<< packetID << ','
				<< source << ','
				<< destination << ','
				<< packetSize << ','
				<< "V" << ','
				<< "-" << ','
				<< "-" << ','
				<< std::endl;

			// write m_outputTrafficInfoBuffer in each terminal interface
			m_network->m_terminalInterfaces.at(-source - 1)->
				m_outputTrafficInfoBuffer.push_back({
					packetID, source, destination,
					packetSize, "V", 0, 0});
		}
	}

	writeTrafficInformation.close();
	writeTrafficData.close();
}

void TrafficOperator::generatePermutation(const int destination)
{
	// write append open TrafficInformation.csv
	std::ofstream writeTrafficInformation(m_trafficFolderPath +
		"TrafficInformation.csv", std::ios::app);
	// write append open TrafficData.csv
	std::ofstream writeTrafficData(m_trafficFolderPath +
		"TrafficData.csv", std::ios::app);

	for (int source{ -1 }; source >= -m_network->getRouterNumber(); --source)
	{
		if (source != destination)
		{
			for (int packetID{}; packetID < g_packetNumber; ++packetID)
			{
				// write TrafficData.csv
				int packetSize{ g_packetSize };
				if (g_packetSizeOption == "random uniform")
					packetSize = uniformDistribution(1, g_packetSize);
				for (int i{}; i < packetSize; ++i)
					writeTrafficData << static_cast<float>(i) << ',';
				writeTrafficData << std::endl;

				// write m_outputTrafficDataBuffer in each terminal interface
				std::vector<float> data{};
				for (int i{}; i < packetSize; ++i)
					data.push_back(i);
				m_network->m_terminalInterfaces.at(-source - 1)->
					m_outputTrafficDataBuffer.push_back(data);

				// write TrafficInformation.csv
				writeTrafficInformation
					<< packetID << ','
					<< source << ','
					<< destination << ','
					<< packetSize << ','
					<< "V" << ','
					<< "-" << ','
					<< "-" << ','
					<< std::endl;

				// write m_outputTrafficInfoBuffer in each terminal interface
				m_network->m_terminalInterfaces.at(-source - 1)->
					m_outputTrafficInfoBuffer.push_back({
						packetID, source, destination,
						packetSize, "V", 0, 0 });
			}
		}
	}

	writeTrafficInformation.close();
	writeTrafficData.close();
}

//void TrafficOperator::generateCustomize()
//{
//	// write append open TrafficInformation.csv
//	std::ofstream writeTrafficInformation(m_trafficFolderPath +
//		"TrafficInformation.csv", std::ios::app);
//	// write append open TrafficData.csv
//	std::ofstream writeTrafficData(m_trafficFolderPath +
//		"TrafficData.csv", std::ios::app);
//
//	std::vector<float> data{ 0, 1, 2, 3, 4 };
//
//	// write TrafficData.csv
//	for (auto& d : data)
//		writeTrafficData << d << ',';
//	writeTrafficData << std::endl;
//
//	// write TrafficInformation.csv
//	writeTrafficInformation
//		<< 0 << ','
//		<< -1 << ','
//		<< -2 << ','
//		<< data.size() << ','
//		<< "V" << ','
//		<< "-" << ','
//		<< "-" << ','
//		<< std::endl;
//
//	writeTrafficInformation.close();
//	writeTrafficData.close();
//}

int TrafficOperator::uniformDistribution(const int min, const int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

void TrafficOperator::updateTrafficInformation()
{
	// read open TrafficInformation.csv
	std::ifstream readPacketInformation(m_trafficFolderPath +
		"TrafficInformation.csv", std::ios::in);
	// write open t_TrafficInformation.csv; tmp file
	std::ofstream writePacketInformation(m_trafficFolderPath +
		"t_TrafficInformation.csv", std::ios::out);
	std::string infoLine{};
	std::istringstream infoLineInString{};
	std::string packetID{}, source{}, destination{}, packetSize{},
		status{}, sentTime{}, receivedTime{};
	// read the head line
	std::getline(readPacketInformation, infoLine);
	// write the head line into tmp file
	writePacketInformation << infoLine << std::endl;
	// read file line by line
	while (std::getline(readPacketInformation, infoLine))
	{
		infoLineInString.str(infoLine);
		// read each field
		std::getline(infoLineInString, packetID, ',');
		std::getline(infoLineInString, source, ',');
		std::getline(infoLineInString, destination, ',');
		std::getline(infoLineInString, packetSize, ',');
		std::getline(infoLineInString, status, ',');
		std::getline(infoLineInString, sentTime, ',');
		std::getline(infoLineInString, receivedTime, ',');

			for (size_t i{}; i < m_network->m_terminalInterfaces.
				at(-stoi(source) - 1)->m_outputTrafficInfoBuffer.size(); ++i)
		{
			if (m_network->m_terminalInterfaces.at(-stoi(source) - 1)
				->m_outputTrafficInfoBuffer.at(i).m_packetID == stoi(packetID) &&
				m_network->m_terminalInterfaces.at(-stoi(source) - 1)
				->m_outputTrafficInfoBuffer.at(i).m_status == "S")
			{
				status = "S";
				sentTime = std::to_string(m_network->m_terminalInterfaces.at(-stoi(source) - 1)
					->m_outputTrafficInfoBuffer.at(i).m_sentTime);
				break;
			}
		}

			for (size_t i{}; i < m_network->m_terminalInterfaces.
				at(-stoi(destination) - 1)->m_inputTrafficInfoBuffer.size(); ++i)
		{
			if (m_network->m_terminalInterfaces.at(-stoi(destination) - 1)
				->m_inputTrafficInfoBuffer.at(i).m_packetID == stoi(packetID) &&
				m_network->m_terminalInterfaces.at(-stoi(destination) - 1)
				->m_inputTrafficInfoBuffer.at(i).m_status == "R")
			{
				status = "R";
				receivedTime = std::to_string(m_network->m_terminalInterfaces.at(-stoi(destination) - 1)
					->m_inputTrafficInfoBuffer.at(i).m_receivedTime);
				break;
			}
		}

		writePacketInformation
			<< packetID << ','
			<< source << ','
			<< destination << ','
			<< packetSize << ','
			<< status << ','
			<< sentTime << ','
			<< receivedTime << ','
			<< std::endl;
	}
	readPacketInformation.close();
	writePacketInformation.close();
	// delete the old file and rename the tmp file to the deleted file
	std::remove((m_trafficFolderPath + "TrafficInformation.csv").c_str());
	std::rename((m_trafficFolderPath + "t_TrafficInformation.csv").c_str(),
		(m_trafficFolderPath + "TrafficInformation.csv").c_str());
}

void TrafficOperator::collectData()
{
	// read open TrafficInformation.csv
	std::ifstream readPacketInformation(m_trafficFolderPath +
		"TrafficInformation.csv", std::ios::in);
	std::string infoLine{};
	std::istringstream infoLineInString{};
	std::string packetID{}, source{}, destination{}, packetSize{},
		status{}, sentTime{}, receivedTime{};
	// read the head line
	std::getline(readPacketInformation, infoLine);
	while (std::getline(readPacketInformation, infoLine))
	{
		infoLineInString.str(infoLine);
		// read each field
		std::getline(infoLineInString, packetID, ',');
		std::getline(infoLineInString, source, ',');
		std::getline(infoLineInString, destination, ',');
		std::getline(infoLineInString, packetSize, ',');
		std::getline(infoLineInString, status, ',');
		std::getline(infoLineInString, sentTime, ',');
		std::getline(infoLineInString, receivedTime, ',');
		if (status != "V")
		{
			if (status == "R")
			{
				if (std::stoi(receivedTime) >= g_warmupCycles
					&& std::stoi(receivedTime) <
					(g_warmupCycles + g_measurementCycles))
				{
					m_trafficData.m_receivedPacketNumber++;
					m_trafficData.m_receivedFlitNumber += std::stof(packetSize);
				}
			}
			if (std::stoi(sentTime) >= g_warmupCycles
				&& std::stoi(sentTime) < (g_warmupCycles + g_measurementCycles))
			{
				m_trafficData.m_sentPacketNumber++;
				m_trafficData.m_sentFlitNumber += std::stof(packetSize);
				if (status == "R")
					m_trafficData.m_accumulatedLatency +=
					(std::stof(receivedTime) - std::stof(sentTime) - 1);
			}
		}
	}
	readPacketInformation.close();
}

void TrafficOperator::calculatePerformance()
{
	float throughput{ m_trafficData.m_receivedFlitNumber
		/ (g_measurementCycles * m_network->getRouterNumber()) },
		demand{ m_trafficData.m_sentFlitNumber
		/ (g_measurementCycles * m_network->getRouterNumber()) },
		latency{ m_trafficData.m_accumulatedLatency
		/ m_trafficData.m_sentPacketNumber };
	std::cout << "************** Network performance **************\n" 
		<< "Throughput: " << throughput << " flit/cycle/node\n"
		<< "Demand: " << demand << " flit/cycle/node\n"
		<< "Average latency: " << latency << " cycles" << std::endl;
}