# SOXIM - Network-on-Chip Simulator

A cycle-accurate Network-on-Chip (NoC) simulator for multi-core processor interconnection networks.

## Features

- **Topologies**: 2D/3D Mesh and Torus networks
- **Routing**: Dimension-Order Routing (DOR), with extensible architecture for ROMM, MAD, VAL
- **Virtual Channels**: Configurable number of virtual channels per port
- **Traffic Patterns**: Random uniform, permutation, customizable injection processes
- **Performance Metrics**: Throughput, latency, saturation analysis

## Project Structure

```
soxim/
├── CMakeLists.txt          # Root CMake configuration
├── README.md               # This file
├── configs/                # Configuration files
│   ├── example.toml        # Example configuration
│   └── test.toml          # Test configuration
├── scripts/                # Python analysis tools
│   ├── analyze.py         # Analyze simulation results
│   ├── sweep.py           # Parameter sweep & saturation curves
│   ├── compare.py         # Compare multiple runs
│   ├── requirements.txt   # Python dependencies
│   └── README.md          # Scripts documentation
└── src/                    # Source code
    ├── CMakeLists.txt
    ├── main.cpp           # Entry point with CLI
    ├── Clock.cpp/h        # Simulation clock
    ├── DataStructures.cpp/h  # Flits, packets, credits
    ├── Link.cpp/h         # Inter-router links
    ├── Port.h             # Router ports
    ├── Router.cpp/h       # Router implementation
    ├── RegularNetwork.cpp/h  # Network topology
    ├── TerminalInterface.cpp/h  # Traffic injection/ejection
    ├── TrafficOperator.cpp/h    # Traffic generation & analysis
    └── external/
        └── toml.hpp       # TOML config parser
```

## Building

### Requirements
- C++20 compatible compiler (GCC 10+, Clang 12+)
- CMake 3.14+ (or use provided Makefile)

### CMake Build (Recommended)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

### Build Types
- `Debug`: Debug symbols, no optimization
- `Release`: Optimized (-O3), with benchmarking
- `RelWithDebInfo`: Optimized with debug symbols

## Usage

### Basic Run

```bash
./soxim                              # Run with default config
./soxim configs/my_config.toml       # Run with custom config
./soxim -o /tmp/results config.toml  # Specify output directory
```

### CLI Options

```bash
./soxim --help     # Show help
./soxim --version  # Show version
```

### Configuration

Edit `configs/example.toml`:

```toml
[topology]
dimension = [8, 8, 1]    # 8x8 2D network
shape = "TORUS"          # MESH or TORUS

[routing]
algorithm = "DOR"        # Dimension-Order Routing

[microarchitecture]
virtual_channel_number = 8
buffer_size = 8

[traffic]
injection_rate = 0.01    # packets per cycle
packet_size = 20          # flits per packet
traffic_pattern = "random uniform"

[cycles]
total = 10000
warmup = 3000
measurement = 3000
```

## Analysis

### Quick Analysis

```bash
cd scripts
pip install -r requirements.txt

# Analyze results
./analyze.py ../build/src/traffic/TrafficInformation.csv

# Generate saturation curve
./sweep.py -c ../configs/example.toml --rates 0.01 0.05 0.1 0.2 0.3

# Compare configurations
./compare.py run1/traffic/ run2/traffic/ -l "Config1" "Config2"
```

See `scripts/README.md` for detailed usage.

## Output

Simulation generates in `traffic/` directory:
- `TrafficInformation.csv`: Packet trace with timestamps
- `TrafficData.csv`: Packet payload data

Console output shows:
- Configuration summary
- Throughput (flits/cycle/node)
- Average latency (cycles)
- Demand (flits/cycle/node)

## Example Workflow

```bash
# 1. Build
mkdir build && cd build
cmake .. && make -j4

# 2. Run simulation
cd src
./soxim ../../configs/example.toml

# 3. Analyze
cd ../../scripts
./analyze.py ../build/src/traffic/

# 4. Parameter sweep
./sweep.py -c ../configs/example.toml -n 10 -o saturation.png
```

## Development

### Code Organization
- **Router**: Virtual channel allocation, switch allocation, routing
- **RegularNetwork**: Topology setup (MESH/TORUS), link connections
- **TerminalInterface**: Traffic injection, packet assembly/disassembly
- **TrafficOperator**: Traffic generation patterns, result analysis

### Adding Features

1. **New Routing Algorithm**: Implement in `Router::computeRoute()`
2. **New Traffic Pattern**: Add to `TrafficOperator::generateTraffic()`
3. **New Topology**: Extend `RegularNetwork` connection methods

## License

MIT License - See LICENSE file

## Contributing

Contributions welcome! Areas for improvement:
- Additional routing algorithms (ROMM, MAD, VAL)
- More traffic patterns
- Performance optimizations
- GUI visualization
- Unit tests

## Acknowledgments

Built for computer architecture research and education.
