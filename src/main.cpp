#include "TrafficOperator.h"
#include "toml.hpp"
#include <filesystem>
#include <cstring>
#include <fstream>
#include <chrono>

using namespace std::string_view_literals;

void printUsage(const char* programName)
{
	std::cout << "Usage: " << programName << " [OPTIONS] [CONFIG_FILE]\n\n"
			  << "Network-on-Chip (NoC) Simulator\n\n"
			  << "Arguments:\n"
			  << "  CONFIG_FILE           Path to TOML configuration file (default: configs/example.toml)\n\n"
			  << "General Options:\n"
			  << "  -h, --help            Show this help message\n"
			  << "  -v, --version         Show version information\n"
			  << "  -q, --quiet           Suppress detailed output\n"
			  << "  -d, --debug           Enable debug output\n\n"
			  << "Simulation Options:\n"
			  << "  -o, --output DIR      Specify output directory for traffic files (default: ./traffic/)\n"
			  << "  -t, --topology TYPE   Override topology type (MESH, TORUS)\n"
			  << "  -a, --algorithm ALGO  Override routing algorithm (DOR, ROMM, MAD, VAL, ODD_EVEN)\n"
			  << "  -r, --rate RATE       Override injection rate (0.0-1.0)\n"
			  << "  -s, --size SIZE       Override packet size (flits)\n"
			  << "  -p, --pattern PATTERN Override traffic pattern (random uniform, permutation)\n"
			  << "  -c, --cycles CYCLES   Override total cycles\n"
			  << "  -w, --warmup CYCLES   Override warmup cycles\n"
			  << "  -m, --measure CYCLES  Override measurement cycles\n\n"
			  << "Output Options:\n"
			  << "  --no-traffic          Skip traffic generation\n"
			  << "  --no-analysis         Skip traffic analysis\n"
			  << "  --save-config FILE    Save current config to file\n"
			  << "  --dry-run             Parse config and show settings, don't run simulation\n\n"
			  << "Examples:\n"
			  << "  " << programName << "                           # Run with default config\n"
			  << "  " << programName << " my_config.toml            # Run with custom config\n"
			  << "  " << programName << " -o /tmp/results config.toml  # Specify output directory\n"
			  << "  " << programName << " -t TORUS -a MAD -r 0.05     # Override topology and algorithm\n"
			  << "  " << programName << " --dry-run config.toml       # Show config without running\n";
}

struct Arguments
{
	std::string configPath{"configs/example.toml"};
	std::string outputDir{"./traffic/"};
	std::string topologyOverride{""};
	std::string algorithmOverride{""};
	std::string patternOverride{""};
	float rateOverride{-1.0f};
	int sizeOverride{-1};
	int totalCyclesOverride{-1};
	int warmupCyclesOverride{-1};
	int measureCyclesOverride{-1};
	std::string saveConfigPath{""};
	bool showHelp{false};
	bool showVersion{false};
	bool quiet{false};
	bool debug{false};
	bool noTraffic{false};
	bool noAnalysis{false};
	bool dryRun{false};
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
		else if (std::strcmp(argv[i], "-q") == 0 || std::strcmp(argv[i], "--quiet") == 0)
		{
			args.quiet = true;
		}
		else if (std::strcmp(argv[i], "-d") == 0 || std::strcmp(argv[i], "--debug") == 0)
		{
			args.debug = true;
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
		else if (std::strcmp(argv[i], "-t") == 0 || std::strcmp(argv[i], "--topology") == 0)
		{
			if (i + 1 < argc)
				args.topologyOverride = argv[++i];
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (std::strcmp(argv[i], "-a") == 0 || std::strcmp(argv[i], "--algorithm") == 0)
		{
			if (i + 1 < argc)
				args.algorithmOverride = argv[++i];
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (std::strcmp(argv[i], "-r") == 0 || std::strcmp(argv[i], "--rate") == 0)
		{
			if (i + 1 < argc)
			{
				try
				{
					args.rateOverride = std::stof(argv[++i]);
				}
				catch (const std::exception&)
				{
					std::cerr << "Error: Invalid rate value: " << argv[i] << "\n";
					args.showHelp = true;
					return args;
				}
			}
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (std::strcmp(argv[i], "-s") == 0 || std::strcmp(argv[i], "--size") == 0)
		{
			if (i + 1 < argc)
			{
				try
				{
					args.sizeOverride = std::stoi(argv[++i]);
				}
				catch (const std::exception&)
				{
					std::cerr << "Error: Invalid size value: " << argv[i] << "\n";
					args.showHelp = true;
					return args;
				}
			}
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (std::strcmp(argv[i], "-p") == 0 || std::strcmp(argv[i], "--pattern") == 0)
		{
			if (i + 1 < argc)
				args.patternOverride = argv[++i];
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (std::strcmp(argv[i], "-c") == 0 || std::strcmp(argv[i], "--cycles") == 0)
		{
			if (i + 1 < argc)
			{
				try
				{
					args.totalCyclesOverride = std::stoi(argv[++i]);
				}
				catch (const std::exception&)
				{
					std::cerr << "Error: Invalid cycles value: " << argv[i] << "\n";
					args.showHelp = true;
					return args;
				}
			}
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (std::strcmp(argv[i], "-w") == 0 || std::strcmp(argv[i], "--warmup") == 0)
		{
			if (i + 1 < argc)
			{
				try
				{
					args.warmupCyclesOverride = std::stoi(argv[++i]);
				}
				catch (const std::exception&)
				{
					std::cerr << "Error: Invalid warmup cycles value: " << argv[i] << "\n";
					args.showHelp = true;
					return args;
				}
			}
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (std::strcmp(argv[i], "-m") == 0 || std::strcmp(argv[i], "--measure") == 0)
		{
			if (i + 1 < argc)
			{
				try
				{
					args.measureCyclesOverride = std::stoi(argv[++i]);
				}
				catch (const std::exception&)
				{
					std::cerr << "Error: Invalid measure cycles value: " << argv[i] << "\n";
					args.showHelp = true;
					return args;
				}
			}
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (std::strcmp(argv[i], "--no-traffic") == 0)
		{
			args.noTraffic = true;
		}
		else if (std::strcmp(argv[i], "--no-analysis") == 0)
		{
			args.noAnalysis = true;
		}
		else if (std::strcmp(argv[i], "--save-config") == 0)
		{
			if (i + 1 < argc)
				args.saveConfigPath = argv[++i];
			else
			{
				std::cerr << "Error: Missing argument for " << argv[i] << "\n";
				args.showHelp = true;
				return args;
			}
		}
		else if (std::strcmp(argv[i], "--dry-run") == 0)
		{
			args.dryRun = true;
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

static void parseConfiguration(const toml::table& table, const Arguments& args)
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

	// Apply CLI overrides
	if (!args.topologyOverride.empty())
		g_shape = args.topologyOverride;
	if (!args.algorithmOverride.empty())
		g_routingAlgorithm = args.algorithmOverride;
	if (!args.patternOverride.empty())
		g_trafficPattern = args.patternOverride;
	if (args.rateOverride >= 0.0f)
		g_injectionRate = args.rateOverride;
	if (args.sizeOverride > 0)
		g_packetSize = args.sizeOverride;
	if (args.totalCyclesOverride > 0)
		g_totalCycles = args.totalCyclesOverride;
	if (args.warmupCyclesOverride > 0)
		g_warmupCycles = args.warmupCyclesOverride;
	if (args.measureCyclesOverride > 0)
		g_measurementCycles = args.measureCyclesOverride;

	// Recalculate derived values
	g_drainCycles = g_totalCycles - g_warmupCycles - g_measurementCycles;
	g_packetNumber = g_totalCycles * g_injectionRate;
}

static void saveConfiguration(const Arguments& args)
{
	if (args.saveConfigPath.empty())
		return;

	std::ofstream file(args.saveConfigPath);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file for writing: " << args.saveConfigPath << "\n";
		return;
	}

	file << "# Configuration saved by SOXIM\n";
	file << "# Generated: " << std::chrono::system_clock::now().time_since_epoch().count() << "\n\n";

	file << "[cycles]\n";
	file << "total = " << g_totalCycles << "\n";
	file << "warmup = " << g_warmupCycles << "\n";
	file << "measurement = " << g_measurementCycles << "\n\n";

	file << "[microarchitecture]\n";
	file << "buffer_size = " << g_bufferSize << "\n";
	file << "virtual_channel_number = " << g_virtualChannelNumber << "\n\n";

	file << "[routing]\n";
	file << "algorithm = \"" << g_routingAlgorithm << "\"\n\n";

	file << "[topology]\n";
	file << "dimension = [ " << g_x << ", " << g_y << ", " << g_z << " ]\n";
	file << "shape = \"" << g_shape << "\"\n\n";

	file << "[traffic]\n";
	file << "alpha = " << g_alpha << "\n";
	file << "beta = " << g_beta << "\n";
	file << "flit_size = " << g_flitSize << "\n";
	file << "injection_process = \"" << g_injectionProcess << "\"\n";
	file << "injection_rate = " << g_injectionRate << "\n";
	file << "packet_size = " << g_packetSize << "\n";
	file << "packet_size_option = \"" << g_packetSizeOption << "\"\n";
	file << "traffic_pattern = \"" << g_trafficPattern << "\"\n";

	file.close();
	std::cout << "Configuration saved to: " << args.saveConfigPath << "\n";
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
		if (!args.quiet)
		{
			std::cout << "************** Simulator configuration **************\n"
				<< table << std::endl;
		}
	}
	catch (const toml::parse_error& err)
	{
		std::cerr << "Parsing failed:\n" << err << "\n";
		return 1;
	}

	parseConfiguration(table, args);

	// Dry run mode - just show config and exit
	if (args.dryRun)
	{
		std::cout << "************** Effective Configuration **************\n";
		std::cout << "[cycles]\n";
		std::cout << "total = " << g_totalCycles << "\n";
		std::cout << "warmup = " << g_warmupCycles << "\n";
		std::cout << "measurement = " << g_measurementCycles << "\n\n";
		std::cout << "[topology]\n";
		std::cout << "dimension = [ " << g_x << ", " << g_y << ", " << g_z << " ]\n";
		std::cout << "shape = \"" << g_shape << "\"\n\n";
		std::cout << "[routing]\n";
		std::cout << "algorithm = \"" << g_routingAlgorithm << "\"\n\n";
		std::cout << "[traffic]\n";
		std::cout << "injection_rate = " << g_injectionRate << "\n";
		std::cout << "packet_size = " << g_packetSize << "\n";
		std::cout << "traffic_pattern = \"" << g_trafficPattern << "\"\n";
		std::cout << "******************************************************\n";
		return 0;
	}

	// Save configuration if requested
	saveConfiguration(args);

	RegularNetwork* network{ new RegularNetwork{} };

	for (int i{}; i < network->getRouterNumber(); ++i)
	{
		TerminalInterface* terminal{ new TerminalInterface{-i - 1} };
		network->connectTerminal(i, terminal);
	}
	network->loadNetworkData();

	if (!args.noTraffic)
	{
		TrafficOperator* trafficOperator{ new TrafficOperator{
			args.outputDir,
			network} };
		trafficOperator->generateTraffic();

		for (Clock clk; clk.get() < g_totalCycles; clk.tick())
			network->runOneCycle();

		if (!args.noAnalysis)
		{
			trafficOperator->analyzeTraffic();
		}

		delete trafficOperator;
		trafficOperator = nullptr;
	}
	else
	{
		// Just run simulation without traffic generation
		for (Clock clk; clk.get() < g_totalCycles; clk.tick())
			network->runOneCycle();
	}

	delete network;
	network = nullptr;

	return 0;
}
