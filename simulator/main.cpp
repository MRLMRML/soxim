#include "TrafficOperator.h"
#include "toml.hpp"

using namespace std::string_view_literals;

static void parseConfiguration(const toml::table& table)
{
	g_x = table["topology"]["dimension"][0].value_or<int>(0);
	g_y = table["topology"]["dimension"][1].value_or<int>(0);
	g_z = table["topology"]["dimension"][2].value_or<int>(0);
	g_shape = table["topology"]["shape"].value_or(""sv);
	g_routingAlgorithm = table["routing"]["algorithm"].value_or(""sv);
	g_virtualChannelNumber = table["microarchitecture"]["virtual_channel_number"].value_or<int>(0);
	g_bufferSize = table["microarchitecture"]["buffer_size"].value_or<int>(0);
	g_flitSize = table["traffic"]["flit_size"].value_or<int>(0);
	g_packetSize = table["traffic"]["packet_size"].value_or<int>(0);
	g_packetSizeOption = table["traffic"]["packet_size_option"].value_or(""sv);
	g_injectionRate = table["traffic"]["injection_rate"].value_or<float>(0);
	g_injectionProcess = table["traffic"]["injection_process"].value_or(""sv);
	g_alpha = table["traffic"]["alpha"].value_or<float>(0);
	g_beta = table["traffic"]["beta"].value_or<float>(0);
	g_trafficPattern = table["traffic"]["traffic_pattern"].value_or(""sv);
	g_totalCycles = table["cycles"]["total"].value_or<int>(0);
	g_warmupCycles = table["cycles"]["warmup"].value_or<int>(0);
	g_measurementCycles = table["cycles"]["measurement"].value_or<int>(0);
	g_drainCycles = g_totalCycles - g_warmupCycles - g_measurementCycles;
	g_packetNumber = g_totalCycles * g_injectionRate;
}

int main(int argc, char* argv[])
{
#if BENCHMARK
	Benchmark benchmark{};
#endif
	
	std::string trafficFolderPath{ "/home/ubuntu/.vs/simulator/simulator/traffic/" };
	mkdir((trafficFolderPath).c_str(), 0777);

	toml::table table{};
	try
	{
		if (argc == 1)
			table = toml::parse_file("/home/ubuntu/.vs/simulator/simulator/configurations/example.toml");
		else
			table = toml::parse_file(argv[1]);
		std::cout << "************** Simulator configuration **************\n"
			<< table << std::endl;
	}
	catch (const toml::parse_error& err)
	{
		std::cerr << "Parsing failed:\n" << err << "\n";
		return 1;
	}

	parseConfiguration(table);

	RegularNetwork* network{ new RegularNetwork{} };

	for (int i{}; i < network->getRouterNumber(); ++i)
	{
		TerminalInterface* terminal{ new TerminalInterface{-i - 1} };
		network->connectTerminal(i, terminal);
	}
	network->loadNetworkData();

	TrafficOperator* trafficOperator{ new TrafficOperator{
		trafficFolderPath,
		network} };
	trafficOperator->generateTraffic();

	for (Clock clk; clk.get() < g_totalCycles; clk.tick())
		network->runOneCycle();

	trafficOperator->analyzeTraffic();

	delete trafficOperator;
	trafficOperator = nullptr;
	delete network;
	network = nullptr;

	return 0;
}
