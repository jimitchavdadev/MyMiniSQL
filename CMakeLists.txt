cmake_minimum_required(VERSION 3.10)
project(MyMiniSQL CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Create executable
add_executable(MyMiniSQL ${SOURCES})

# Create databases directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/databases)