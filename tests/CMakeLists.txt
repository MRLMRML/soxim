# CMakeLists.txt : Test configuration
#

# Enable testing
enable_testing()

# Include GoogleTest
include(GoogleTest)

# Add unit tests subdirectory
add_subdirectory(unit)
