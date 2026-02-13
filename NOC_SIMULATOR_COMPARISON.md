# Network-on-Chip Simulator Comparison: SOXIM vs Others

## Overview

This document compares SOXIM with other popular Network-on-Chip (NoC) simulators available in academia and open-source communities.

## Comparison Table

| Feature | SOXIM | Noxim | Booksim | HNOCS | NOCulator |
|---------|-------|-------|---------|-------|-----------|
| **Language** | C++20 | SystemC | C++ | C++ | C# |
| **Simulation Type** | Cycle-accurate | Cycle-accurate | Cycle-accurate | Cycle-accurate | Cycle-accurate |
| **Topologies** | 2D/3D Mesh, Torus | 2D Mesh | Mesh, Torus, Fat Tree | Heterogeneous | Mesh, Torus, Ring |
| **Routing Algorithms** | DOR, ROMM, MAD, VAL, Odd-Even | XY, Odd-Even | Various | Custom | Various |
| **Virtual Channels** | ✅ Configurable | ✅ | ✅ | ✅ | ✅ |
| **Traffic Patterns** | Random uniform, permutation, periodic, Bernoulli, Markov | Various | Various | Custom | Various |
| **Performance Metrics** | Throughput, latency, saturation analysis | Performance analysis | Detailed statistics | Performance analysis | Performance models |
| **Programming Model** | Standalone C++ | SystemC | Standalone C++ | Modular C++ | C#/.NET |
| **Configuration** | TOML files | Configuration files | Configuration files | Configuration files | Configuration files |
| **Visualization** | Python analysis suite | Limited | Limited | Limited | Limited |
| **Testing** | 128+ Google Test cases | Limited | Limited | Limited | Limited |
| **Documentation** | Comprehensive README, CLI docs | Academic papers | Academic documentation | Research papers | Limited |
| **License** | MIT | Academic | Academic | Open source | Open source |

## Detailed Feature Comparison

### SOXIM Advantages

**Modern C++ Implementation**
- Uses C++20 with modern features and best practices
- RAII memory management for safety
- Template-based architecture for flexibility

**Comprehensive Testing**
- 128+ unit tests with Google Test framework
- Extensive test coverage across all components
- Automated CI/CD pipeline

**Advanced Analysis Tools**
- 6 Python scripts for comprehensive visualization
- Automated parameter sweeps and comparisons
- Saturation curve analysis
- Network topology visualization

**Professional Tooling**
- Docker containerization support
- Modern build system (CMake)
- Professional documentation

### Noxim (SystemC-based)

**Strengths**
- Widely cited in academic research
- SystemC integration for hardware modeling
- Power and thermal modeling capabilities

**Limitations**
- SystemC learning curve
- Less modern C++ practices
- Limited visualization tools

### Booksim (Stanford)

**Strengths**
- Extensive topology support
- Well-established in academia
- Detailed performance statistics

**Limitations**
- Older C++ codebase
- Limited modern tooling
- Steeper learning curve

### HNOCS (Heterogeneous NoC Simulator)

**Strengths**
- Focus on heterogeneous architectures
- Modular design
- Research-focused features

**Limitations**
- Limited documentation
- Smaller community
- Fewer pre-built configurations

## Performance Comparison

### Simulation Speed
- **SOXIM**: Optimized C++20 with -O3 compilation
- **Noxim**: SystemC overhead may impact performance
- **Booksim**: Mature but potentially slower due to older optimizations

### Accuracy
- All compared simulators are cycle-accurate
- SOXIM includes comprehensive validation through unit testing
- Booksim and Noxim have extensive academic validation

## Use Case Recommendations

### Choose SOXIM for:
- Modern C++ development and research
- Comprehensive testing and validation
- Advanced visualization and analysis
- Educational purposes with clear documentation
- Projects requiring Docker containerization

### Choose Noxim for:
- SystemC integration requirements
- Power and thermal modeling
- Academic research with established benchmarks

### Choose Booksim for:
- Complex topology requirements
- Established academic comparisons
- Detailed statistical analysis

## Development Experience

### SOXIM
- **Build System**: Modern CMake with presets
- **Testing**: Google Test with comprehensive coverage
- **Documentation**: Professional README, CLI documentation
- **Dependencies**: Minimal external dependencies

### Other Simulators
- **Build Systems**: Often custom Makefiles or older build systems
- **Testing**: Limited or academic-focused testing
- **Documentation**: Primarily academic papers
- **Dependencies**: May require SystemC or other specialized libraries

## Conclusion

SOXIM stands out for its modern development practices, comprehensive testing, and advanced analysis capabilities. While established simulators like Noxim and Booksim have strong academic foundations, SOXIM offers a more professional and maintainable codebase suitable for both research and educational use.

For new NoC research projects, SOXIM provides an excellent balance of modern tooling, comprehensive features, and professional development practices.