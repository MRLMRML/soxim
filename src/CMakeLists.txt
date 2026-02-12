# CMakeLists.txt : soxim simulator executable
#

# Source files
set(SOXIM_SOURCES
    main.cpp
    Clock.cpp
    DataStructures.cpp
    Link.cpp
    RegularNetwork.cpp
    Register.cpp
    Router.cpp
    TerminalInterface.cpp
    TrafficOperator.cpp
)

set(SOXIM_HEADERS
    Clock.h
    DataStructures.h
    Link.h
    Parameters.h
    Port.h
    RegularNetwork.h
    Register.h
    Router.h
    TerminalInterface.h
    TrafficOperator.h
)

# Main executable
add_executable(soxim ${SOXIM_SOURCES})

# Compiler features
target_compile_features(soxim PRIVATE cxx_std_20)

# Include directories
target_include_directories(soxim PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# Link libraries (std::filesystem needs this on some compilers)
target_link_libraries(soxim PRIVATE stdc++fs)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(soxim PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Definitions based on build type
target_compile_definitions(soxim PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:BENCHMARK>
    $<$<CONFIG:RelWithDebInfo>:BENCHMARK>
)

# Installation
install(TARGETS soxim
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Copy configs to build directory for development
add_custom_command(TARGET soxim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/configs
    $<TARGET_FILE_DIR:soxim>/configs
)

# Testing (optional)
if(BUILD_TESTING)
    enable_testing()
    add_test(NAME soxim_help 
        COMMAND soxim --help
    )
    add_test(NAME soxim_version
        COMMAND soxim --version
    )
endif()
