# SOXIM User Guide

## Overview

SOXIM is a high-performance, cycle-accurate Network-on-Chip (NoC) simulator for multi-core processor interconnection networks research and education.

## Quick Start

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

## Configuration

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

## CLI Options

### General Options
- `-h, --help` - Show help message
- `-v, --version` - Show version information
- `-q, --quiet` - Suppress detailed output
- `-d, --debug` - Enable debug output

### Simulation Options
- `-t, --topology` - Network topology (MESH/TORUS)
- `-a, --algorithm` - Routing algorithm (DOR/ROMM/MAD/VAL/ODD_EVEN)
- `-r, --rate` - Injection rate (0.0-1.0)
- `-s, --size` - Packet size in flits
- `-c, --cycles` - Total simulation cycles

## Analysis Tools

### Python Visualization Suite
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

## Architecture

### Core Components
- **Router Pipeline**: 6-stage pipeline (Receive Flit, Receive Credit, Compute Route, VC Allocation, Switch Allocation, Traverse Switch)
- **Virtual Channel Management**: Configurable buffers and flow control
- **Traffic Generation**: Multiple statistical patterns with reproducible randomness
- **Performance Analysis**: Real-time metrics collection and post-simulation analysis

### Supported Features
- **Topologies**: 2D/3D Mesh and Torus networks
- **Routing Algorithms**: DOR, ROMM, MAD, VAL, Odd-Even
- **Virtual Channels**: Configurable virtual channels per port
- **Traffic Patterns**: Random uniform, permutation, periodic, Bernoulli, Markov injection processes

## Testing

```bash
# Run all tests
./scripts/run_tests.sh

# Build and run specific tests
cd build && make test_data_structures
./tests/unit/test_data_structures

# Test coverage
cd build && make coverage
```

## Docker Support

```bash
# Build and run in container
docker build -t soxim:latest .
docker run --rm soxim:latest configs/example.toml

# With volume mount for results
docker run --rm -v $(pwd)/results:/app/traffic soxim:latest configs/example.toml
```

## Troubleshooting

### Common Issues
- **Build failures**: Ensure C++20 compiler and CMake 3.14+
- **Memory issues**: Check system resources for large simulations
- **Configuration errors**: Validate TOML file syntax

### Performance Tips
- Use `-O3` optimization for production runs
- Adjust buffer sizes based on network size
- Monitor memory usage for large-scale simulations

## Contributing

See [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines on contributing to SOXIM.

## License

MIT License - See LICENSE file