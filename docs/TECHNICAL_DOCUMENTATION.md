# SOXIM Technical Documentation

## Architecture Overview

SOXIM is built with a modular C++20 architecture designed for performance, maintainability, and extensibility.

### Core Components

#### Data Structures
- **Flit**: Basic data unit with headers and payload
- **Packet**: Collection of flits with routing information
- **Credit**: Flow control mechanism for virtual channels
- **Coordinate**: Network position representation

#### Router Pipeline
6-stage pipeline implementation:
1. **Receive Flit**: Incoming flit processing
2. **Receive Credit**: Credit-based flow control
3. **Compute Route**: Routing algorithm execution
4. **VC Allocation**: Virtual channel assignment
5. **Switch Allocation**: Crossbar switch configuration
6. **Traverse Switch**: Data transmission

#### Network Topology
- **RegularNetwork**: Base class for topology management
- **MeshNetwork**: 2D/3D mesh implementation
- **TorusNetwork**: 2D/3D torus with wrap-around links

### Routing Algorithms

#### Dimension-Order Routing (DOR)
- Deterministic XY routing for mesh networks
- Minimal path selection
- Deadlock-free for mesh topologies

#### Randomized Oblivious Multi-phase Minimal (ROMM)
- Multi-phase routing with randomization
- Load balancing capabilities
- Minimal path selection

#### Minimal Adaptive (MAD)
- Adaptive routing based on network conditions
- Dynamic path selection
- Congestion avoidance

#### Valiant's Randomized Algorithm (VAL)
- Two-phase routing via random intermediate node
- Load distribution across network
- Deadlock avoidance

#### Odd-Even Adaptive
- Adaptive routing with odd-even turn restrictions
- Deadlock prevention
- Dynamic path selection

### Traffic Generation

#### Patterns Supported
- **Random Uniform**: Equal probability to all destinations
- **Permutation**: Specific destination patterns
- **Periodic**: Time-based traffic patterns
- **Bernoulli**: Probabilistic injection
- **Markov**: State-based traffic modeling

### Testing Framework

#### Test Categories
- **Data Structures**: 29 tests for core data types
- **Router Pipeline**: 30 tests for router functionality
- **Traffic Generation**: 22 tests for traffic patterns
- **Routing Algorithms**: 13 tests for routing logic
- **Topology**: 10 tests for network structures
- **Clock & Registers**: 17 tests for timing components

#### Test Coverage
- Comprehensive unit test coverage
- Edge case testing
- Performance validation
- Memory safety verification

## Performance Characteristics

### Memory Management
- RAII patterns for automatic resource management
- Smart pointers for ownership semantics
- Efficient data structure design

### Optimization Features
- C++20 modern features utilization
- -O3 compilation optimization
- Cache-friendly data layouts
- Minimal memory allocations

### Scalability
- Configurable network sizes
- Efficient parallel simulation
- Memory-efficient data structures

## Configuration System

### TOML Configuration
- Human-readable configuration format
- Hierarchical configuration structure
- Validation and error checking

### Command Line Interface
- Flexible parameter overrides
- Comprehensive help system
- Debug and verbose modes

## Analysis Tools

### Python Visualization Suite
- **visualize.py**: Comprehensive result analysis
- **sweep.py**: Parameter sweep automation
- **compare.py**: Multi-configuration comparison
- **analyze.py**: Statistical analysis
- **topology.py**: Network visualization
- **latency.py**: Latency heatmap generation

### Output Formats
- CSV data export
- PNG visualization generation
- Interactive HTML reports
- Statistical summaries

## Build System

### CMake Configuration
- Modern CMake practices
- Dependency management
- Cross-platform compatibility
- Test integration

### Dependencies
- Google Test for unit testing
- TOML++ for configuration parsing
- Python 3.8+ for analysis tools

## Development Guidelines

### Code Standards
- C++20 compliance
- Google C++ Style Guide adherence
- Comprehensive documentation
- Unit test coverage

### Contribution Process
- Feature branch workflow
- Code review requirements
- Testing requirements
- Documentation updates

## Known Limitations

### Current Limitations
- Limited to 2D/3D mesh and torus topologies
- No support for irregular topologies
- Basic power modeling only
- Limited thermal modeling

### Future Enhancements
- Additional topology support
- Advanced power/thermal modeling
- GUI interface development
- Distributed simulation capabilities

## References

### Academic References
- Dally, W. J., & Towles, B. (2004). Principles and Practices of Interconnection Networks.
- Balfour, J., & Dally, W. J. (2006). Design tradeoffs for tiled CMP on-chip networks.
- Various NoC research papers and benchmarks.

### Technical Standards
- C++20 ISO/IEC 14882:2020
- TOML v1.0.0 specification
- Google Test framework documentation