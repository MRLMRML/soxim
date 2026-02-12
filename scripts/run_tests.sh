#!/bin/bash
#
# SOXIM Unit Test Runner
#
# This script builds and runs all unit tests for the SOXIM project.
#

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
BUILD_DIR="build"
TEST_DIR="$BUILD_DIR/tests/unit"
CMAKE_BIN="$HOME/.local/cmake/bin/cmake"

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Check if cmake is available
if [ ! -f "$CMAKE_BIN" ]; then
    print_error "CMake not found at $CMAKE_BIN"
    print_error "Please install CMake or update the CMAKE_BIN path"
    exit 1
fi

# Add cmake to PATH
export PATH="$HOME/.local/cmake/bin:$PATH"

# Check if we're in the soxim directory
if [ ! -f "CMakeLists.txt" ]; then
    print_error "CMakeLists.txt not found in current directory"
    print_error "Please run this script from the soxim project root directory"
    exit 1
fi

# Clean build directory if requested
if [ "$1" = "--clean" ]; then
    print_status "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    print_status "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# Configure the project
if [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
    print_status "Configuring project with CMake..."
    cd "$BUILD_DIR"
    cmake .. -DBUILD_TESTS=ON
    cd ..
fi

# Build the project
print_status "Building project..."
cd "$BUILD_DIR"
make -j$(nproc)
cd ..

# Check if tests were built
if [ ! -d "$TEST_DIR" ]; then
    print_error "Test directory not found: $TEST_DIR"
    print_error "Tests may not have been built"
    exit 1
fi

# Run all tests
print_status "Running unit tests..."
cd "$TEST_DIR"

# List of test executables
TESTS=(
    "test_data_structures"
    "test_clock"
    "test_register"
    "test_routing"
    "test_topology"
)

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

for test in "${TESTS[@]}"; do
    if [ -f "./$test" ]; then
        TOTAL_TESTS=$((TOTAL_TESTS + 1))
        print_status "Running $test..."
        
        if ./$test 2>&1 | grep -q "PASSED"; then
            PASSED_TESTS=$((PASSED_TESTS + 1))
            print_success "$test passed"
        else
            FAILED_TESTS=$((FAILED_TESTS + 1))
            print_error "$test failed"
        fi
    else
        print_warning "Test executable $test not found"
    fi
done

cd ../..

# Print summary
echo ""
echo "=========================================="
echo "Test Summary"
echo "=========================================="
echo -e "Total tests:    ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed tests:   ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed tests:   ${RED}$FAILED_TESTS${NC}"
echo "=========================================="

if [ $FAILED_TESTS -eq 0 ]; then
    print_success "All tests passed!"
    exit 0
else
    print_error "Some tests failed!"
    exit 1
fi
