# SOXIM - Network-on-Chip Simulator

A high-performance, cycle-accurate Network-on-Chip (NoC) simulator for multi-core processor interconnection networks research and education.

## 🚀 Features

### Core Simulation Engine
- **Topologies**: 2D/3D Mesh and Torus networks
- **Routing Algorithms**: 
  - Dimension-Order Routing (DOR)
  - Randomized Oblivious Multi-phase Minimal (ROMM)
  - Minimal Adaptive (MAD)
  - Valiant's Randomized Algorithm (VAL)
  - Odd-Even Adaptive routing
- **Virtual Channels**: Configurable virtual channels per port
- **Traffic Patterns**: Random uniform, permutation, periodic, Bernoulli, Markov injection processes
- **Performance Metrics**: Throughput, latency, saturation analysis

### Advanced Analysis Tools
- **Python Visualization Suite**: 6 comprehensive analysis scripts
- **Saturation Curve Analysis**: Automated parameter sweeps
- **Network Topology Visualization**: Interactive network diagrams
- **Performance Comparison**: Multi-configuration analysis
- **Latency Heatmaps**: Spatial traffic analysis

### Quality Assurance
- **128+ Unit Tests**: Comprehensive test coverage using Google Test
- **CI/CD Pipeline**: Automated testing, code coverage, Docker support
- **Memory Safety**: Manual memory management with RAII patterns
- **Performance Optimized**: C++20 with -O3 optimization

## 📦 Quick Start

### Prerequisites
- C++20 compatible compiler (GCC 10+, Clang 12+)
- CMake 3.14+
- Python 3.8+ (for analysis tools)

### Build & Run
```bash
# Clone and build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run simulation
./soxim configs/example.toml

# Run with custom parameters
./soxim -t MESH -a DOR -r 0.05 configs/example.toml
```

### Analysis & Visualization
```bash
cd scripts
pip install -r requirements.txt

# Generate comprehensive analysis
./visualize.py ../build/src/traffic/ -o docs/figures/

# Run parameter sweep
./sweep.py -c ../configs/example.toml --rates 0.01 0.05 0.1 0.2 0.3

# Compare algorithms
./compare.py run1/traffic/ run2/traffic/ -l "DOR" "ROMM"
```


## 🏗️ Architecture

```
soxim/
├── src/                    # Core simulation engine
│   ├── Router.cpp/h        # Router with pipeline stages
│   ├── RegularNetwork.cpp/h # Topology management
│   ├── TerminalInterface.cpp/h # Traffic injection/ejection
│   ├── TrafficOperator.cpp/h # Traffic generation & analysis
│   └── DataStructures.cpp/h # Flits, packets, credits
├── tests/unit/             # 128+ comprehensive tests
├── scripts/                # Python analysis tools
└── configs/               # TOML configuration files
```

### Key Components
- **Router Pipeline**: 6-stage pipeline (Receive Flit, Receive Credit, Compute Route, VC Allocation, Switch Allocation, Traverse Switch)
- **Virtual Channel Management**: Configurable buffers and flow control
- **Traffic Generation**: Multiple statistical patterns with reproducible randomness
- **Performance Analysis**: Real-time metrics collection and post-simulation analysis

## 🔧 Configuration

Edit `configs/example.toml`:

```toml
[topology]
dimension = [8, 8, 1]    # 8x8 2D network
shape = "MESH"           # MESH or TORUS

[routing]
algorithm = "DOR"        # DOR, ROMM, MAD, VAL, ODD_EVEN

[microarchitecture]
virtual_channel_number = 8
buffer_size = 8

[traffic]
injection_rate = 0.01    # packets per cycle
packet_size = 20         # flits per packet
traffic_pattern = "random uniform"

[cycles]
total = 10000
warmup = 3000
measurement = 3000
```

## 🧪 Testing

```bash
# Run all tests
./scripts/run_tests.sh

# Build and run specific tests
cd build && make test_data_structures
./tests/unit/test_data_structures

# Test coverage
cd build && make coverage
```

**Test Coverage:**
- Data Structures: 29 tests
- Router Pipeline: 30 tests  
- Traffic Generation: 22 tests
- Routing Algorithms: 13 tests
- Topology: 10 tests
- Clock & Registers: 17 tests

## 🐳 Docker Support

```bash
# Build and run in container
docker build -t soxim:latest .
docker run --rm soxim:latest configs/example.toml

# With volume mount for results
docker run --rm -v $(pwd)/results:/app/traffic soxim:latest configs/example.toml
```


## 🔬 Research Applications

SOXIM is ideal for:
- Network-on-Chip architecture research
- Routing algorithm development and comparison
- Traffic pattern analysis
- Performance modeling of multi-core systems
- Educational demonstrations of NoC principles

## 📄 Documentation

- [CLI Options](CLI_OPTIONS.md) - Complete command-line interface documentation
- [Implementation Summary](IMPLEMENTATION_SUMMARY.md) - Technical architecture details
- [Testing Documentation](tests/README.md) - Test suite organization and usage
- [Bug Fix Summary](BUG_FIX_SUMMARY.md) - Critical issues resolved

## 🤝 Contributing

Contributions welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

**Areas for Enhancement:**
- Additional routing algorithms
- More traffic patterns
- GUI visualization interface
- Performance optimizations

## 📜 License

MIT License - See LICENSE file

## 🙏 Acknowledgments

Built for computer architecture research and education. Special thanks to the academic community for feedback and testing.