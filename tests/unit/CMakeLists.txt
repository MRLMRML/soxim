# CMakeLists.txt : Unit tests
#

# Common include directories for tests
set(TEST_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/external
    ${GTEST_INCLUDE_DIRS}
)

# Helper function to create a test
function(add_soxim_test test_name test_source)
    add_executable(${test_name} ${test_source})

    target_include_directories(${test_name} PRIVATE ${TEST_INCLUDE_DIRS})

    target_link_libraries(${test_name}
        GTest::gtest_main
        GTest::gtest
    )

    # Link against all soxim source files
    target_sources(${test_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/src/DataStructures.cpp
        ${CMAKE_SOURCE_DIR}/src/Clock.cpp
        ${CMAKE_SOURCE_DIR}/src/Register.cpp
        ${CMAKE_SOURCE_DIR}/src/Link.cpp
        ${CMAKE_SOURCE_DIR}/src/Router.cpp
        ${CMAKE_SOURCE_DIR}/src/TerminalInterface.cpp
        ${CMAKE_SOURCE_DIR}/src/RegularNetwork.cpp
        ${CMAKE_SOURCE_DIR}/src/TrafficOperator.cpp
    )

    # Compiler features
    target_compile_features(${test_name} PRIVATE cxx_std_20)

    # Compiler options
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${test_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()

    # Add to CTest
    gtest_discover_tests(${test_name})
endfunction()

# Add individual test files
add_soxim_test(test_data_structures test_data_structures.cpp)
add_soxim_test(test_clock test_clock.cpp)
add_soxim_test(test_register test_register.cpp)
add_soxim_test(test_routing test_routing.cpp)
add_soxim_test(test_topology test_topology.cpp)
add_soxim_test(test_router test_router.cpp)
add_soxim_test(test_traffic_operator test_traffic_operator.cpp)
add_soxim_test(test_routing_algorithms test_routing_algorithms.cpp)
