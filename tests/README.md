# SOXIM Unit Tests

This directory contains unit tests for the SOXIM Network-on-Chip Simulator.

## Overview

The unit tests are written using Google Test (gtest) framework and cover the following components:

- **Data Structures**: Tests for Flit, Packet, Credit, Coordinate, and other data structures
- **Clock**: Tests for the cycle counter and timing mechanisms
- **Register**: Tests for input/output registers
- **Routing**: Tests for routing algorithms (DOR) and coordinate operations
- **Topology**: Tests for network topology creation (MESH/TORUS)

## Directory Structure

```
tests/
├── unit/              # Unit tests
│   ├── test_data_structures.cpp
│   ├── test_clock.cpp
│   ├── test_register.cpp
│   ├── test_routing.cpp
│   ├── test_topology.cpp
│   └── CMakeLists.txt
├── integration/       # Integration tests (future)
├── regression/        # Regression tests (future)
└── README.md          # This file
```

## Building and Running Tests

### Prerequisites

- CMake 3.14 or higher
- C++20 compatible compiler (GCC 10+ or Clang 10+)
- Google Test (automatically downloaded by CMake)

### Quick Start

```bash
# From the project root directory
cd /home/mi/soxim

# Run all tests using the provided script
./scripts/run_tests.sh
```

### Manual Build and Run

```bash
# From the project root directory
mkdir -p build && cd build
cmake .. -DBUILD_TESTS=ON
make -j$(nproc)

# Run tests
cd tests/unit
./test_data_structures
./test_clock
./test_register
./test_routing
./test_topology

# Or run all tests with ctest
cd ../..
ctest --output-on-failure
```

### Clean Build

```bash
# Clean and rebuild
./scripts/run_tests.sh --clean
```

## Test Coverage

### Data Structures Tests (29 tests)

- **FlitTest**: Construction, equality operator
- **PacketTest**: Construction
- **CreditTest**: Construction
- **VirtualChannelStateTest**: Enum values, output operator
- **ControlFieldTest**: Default construction
- **PriorityTableEntryTest**: Construction, equality
- **ConnectionTest**: Construction
- **CoordinateTest**: Construction, operations, arithmetic
- **TrafficDataTest**: Construction
- **TrafficInformationEntryTest**: Construction
- **FlitTypeTest**: Output operator

### Clock Tests (9 tests)

- **DefaultConstruction**: Initial state
- **TickOperation**: Incrementing the clock
- **SetOperation**: Adding to interval
- **TriggerDefaultInterval**: Trigger with default interval
- **TriggerCustomInterval**: Trigger with custom interval
- **ZeroInterval**: Trigger with zero interval
- **FractionalInterval**: Trigger with fractional interval
- **MultipleTicksAndResets**: Multiple operations
- **TriggerAfterReset**: Trigger behavior after reset

### Register Tests (8 tests)

- **FlitOperations**: Push and pop flits
- **CreditOperations**: Push and pop credits
- **EnableFlags**: Enable/disable flags
- **MultipleOperations**: Multiple push/pop operations
- **EmptyState**: Empty state after operations
- **DifferentFlitTypes**: Head, body, tail flits
- **CreditIsTailFlag**: Credit tail flag

### Routing Tests (7 tests)

- **Mesh2DRouting**: 2D mesh topology
- **Torus2DRouting**: 2D torus topology
- **Mesh3DRouting**: 3D mesh topology
- **CoordinateConversion**: Coordinate to ID conversion
- **SelfRouting**: Same source and destination
- **CoordinateOperations**: Increment/decrement operations
- **CoordinateWrapAround**: Wrap-around in torus

### Topology Tests (10 tests)

- **Mesh2DConstruction**: 2D mesh topology creation
- **Torus2DConstruction**: 2D torus topology creation
- **Mesh3DConstruction**: 3D mesh topology creation
- **Torus3DConstruction**: 3D torus topology creation
- **SmallNetwork1x1**: 1x1 network
- **DifferentNetworkSizes**: Various network sizes
- **RouterProperties**: Router configuration
- **LinkCreation**: Link creation
- **TerminalInterfaceConnection**: Terminal interface connection
- **CoordinateToIDConversion**: Coordinate to ID conversion

## Adding New Tests

### Creating a New Test File

1. Create a new test file in the `tests/unit/` directory
2. Add the test to `tests/unit/CMakeLists.txt`:

```cmake
add_soxim_test(test_new_component test_new_component.cpp)
```

3. Write your tests using Google Test macros:

```cpp
#include <gtest/gtest.h>
#include "YourComponent.h"

TEST(YourComponentTest, TestName)
{
    // Test code here
    EXPECT_EQ(expected, actual);
}
```

### Test Naming Convention

- Test fixture names: `ComponentNameTest`
- Test names: `DescriptiveTestName`
- Use descriptive names that explain what is being tested

### Best Practices

1. **Isolation**: Each test should be independent
2. **Clear assertions**: Use descriptive assertion messages
3. **Coverage**: Test both normal and edge cases
4. **No side effects**: Tests should not modify global state
5. **Fast**: Tests should run quickly

## Continuous Integration

The tests can be integrated into CI/CD pipelines:

```yaml
# Example GitHub Actions workflow
name: Tests
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y cmake g++
      - name: Build and run tests
        run: ./scripts/run_tests.sh
```

## Debugging Tests

### Verbose Output

```bash
# Run a specific test with verbose output
./test_data_structures --gtest_verbose
```

### Running Specific Tests

```bash
# Run only tests matching a pattern
./test_data_structures --gtest_filter="FlitTest.*"
```

### Debugging with GDB

```bash
# Debug a specific test
gdb ./test_data_structures
(gdb) run --gtest_filter="FlitTest.HeadFlitConstruction"
```

## Troubleshooting

### Tests Not Found

- Ensure CMake is configured with `-DBUILD_TESTS=ON`
- Rebuild the project: `make clean && make`

### Linker Errors

- Check that all source files are included in `tests/unit/CMakeLists.txt`
- Ensure C++20 is enabled

### Test Failures

- Check the test output for specific failure messages
- Use `--gtest_filter` to isolate failing tests
- Verify the expected behavior matches the implementation

## Future Work

- Add integration tests for end-to-end scenarios
- Add regression tests for known bugs
- Add performance benchmarks
- Add code coverage reporting
- Add fuzzing tests for edge cases

## Contributing

When adding new tests:

1. Follow the existing test structure and naming conventions
2. Add tests for all new functionality
3. Ensure tests pass before submitting
4. Update this README if adding new test categories

## License

These tests are part of the SOXIM project and follow the same license as the main project.
