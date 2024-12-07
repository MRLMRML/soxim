# CMakeList.txt : CMake project for simulator, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (simulator "main.cpp" "Router.h" "Router.cpp" "Port.h" "DataStructures.h" "Parameters.h" "DataStructures.cpp" "Register.h" "Register.cpp" "Link.h" "Link.cpp" "Clock.h" "Clock.cpp" "TerminalInterface.h" "TerminalInterface.cpp" "RegularNetwork.h" "RegularNetwork.cpp" "TrafficOperator.h" "TrafficOperator.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET simulator PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
