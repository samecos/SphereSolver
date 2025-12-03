cmake_minimum_required(VERSION 3.10)
project(SphereSolver)

set(CMAKE_CXX_STANDARD 17)

# Create src directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src)

add_executable(SphereSolver src/main.cpp src/SphereUtils.cpp)
