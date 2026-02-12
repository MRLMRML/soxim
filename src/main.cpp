#include "TrafficOperator.h"
#include "toml.hpp"
#include <filesystem>
#include <cstring>

using namespace std::string_view_literals;

void printUsage(const char* programName)
{
	std::cout << "Usage: " << programName << " [OPTIONS] [CONFIG_FILE]\n\n"
			  << "Network-on-Chip (NoC) Simulator\n\n"
			  << "Arguments:\n"
			  << "  CONFIG_FILE           Path to TOML configuration file (default: configs/example.toml)\n\n"
			  << "Options:\n"
			  << "  -h, --help            Show this help message\n"
			  << "  -o, --output DIR      Specify output directory for traffic files (default: ./traffic/)\n"
			  << "  -v, --version         Show version information\n\n"
			  << "Examples:\n"
			  << "  " << programName << "                           # Run with default config\n"
			  << "  " << programName << " my_config.toml            # Run with custom config\n"
			  << "  " << programName << " -o /tmp/results config.toml  # Specify output directory\n";
}

struct Arguments
{
	std::string configPath{"configs/example.toml"};
	std::string outputDir{"./traffic/"};
	bool showHelp{false};
	bool showVersion{false};
};

Arguments parseArguments(int argc, char* argv[])
{
	Arguments args;
	
	for (int i{1}; i < argc; ++i)
	{
		if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0)
		{
			args.showHelp = true;
			return args;
		}
		else if (std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--version") == 0)
		{
			args.showVersion = true;
			return args;
		}
		else if (std::strcmp(argv[i], "-o") == 0 || std::strcmp(argv[i], "--output") == 0)
		{
			if (i + 1 < argc)
			{
				args.outputDir = argv[++i];
				// Ensure trailing slash
				if (!args.outputDir.empty() && args.outputDir.back() != '/')
					args.outputDir += '/';
			}
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (argv[i][0] == '-')
		{
			std::cerr << "Error: Unknown option: " << argv[i] << "\n";
			args.showHelp = true;
			return args;
		}
		else
		{
			// Assume it's the config file path
			args.configPath = argv[i];
		}
	}
	
	return args;
}

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
	Arguments args{parseArguments(argc, argv)};
	
	if (args.showHelp)
	{
		printUsage(argv[0]);
		return 0;
	}
	
	if (args.showVersion)
	{
		std::cout << "soxim - Network-on-Chip Simulator v1.0\n";
		return 0;
	}
	
#if BENCHMARK
	Benchmark benchmark{};
#endif
	
	// Create output directory if it doesn't exist
	std::filesystem::create_directories(args.outputDir);

	toml::table table{};
	try
	{
		table = toml::parse_file(args.configPath);
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
		args.outputDir,
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
