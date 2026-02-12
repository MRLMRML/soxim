# CMakeLists.txt : Top-level CMake project for soxim
# Network-on-Chip Simulator
#
cmake_minimum_required(VERSION 3.14)

# Project metadata
project(soxim
    VERSION 1.0.0
    DESCRIPTION "Network-on-Chip (NoC) Simulator"
    LANGUAGES CXX
)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)

# Compiler flags
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

# Debug vs Release flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DBENCHMARK")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -DBENCHMARK")

# Include source
add_subdirectory(src)

# Install configuration
include(GNUInstallDirs)
install(DIRECTORY configs/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/soxim/configs
)

# CPack configuration for packaging
set(CPACK_PACKAGE_NAME "soxim")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
include(CPack)
