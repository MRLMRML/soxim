# Dockerfile for SOXIM - System-on-Chip Interconnect Modeling
FROM ubuntu:22.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    clang \
    python3 \
    python3-pip \
    python3-venv \
    git \
    doxygen \
    graphviz \
    clang-format \
    cppcheck \
    lcov \
    gcovr \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Setup Python environment
RUN pip3 install --upgrade pip && \
    pip3 install -r scripts/requirements.txt

# Build the project
RUN mkdir -p build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON && \
    make -j$(nproc)

# Run tests
RUN cd build/tests/unit && \
    for test in test_data_structures test_clock test_register test_routing test_topology test_router test_traffic_operator test_routing_algorithms; do \
        echo "Running $test..."; \
        ./$test; \
    done

# Create entrypoint
ENTRYPOINT ["/app/build/soxim"]

# Default command
CMD ["--help"]
