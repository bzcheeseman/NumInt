cmake_minimum_required(VERSION 2.5)
project(NumericalIntegration)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I /usr/local/include/eigen3 -I /usr/include/python2.7")

find_package(gflags REQUIRED)

set(SOURCE_FILES main.cpp Solver.hpp)
add_executable(NumericalIntegration ${SOURCE_FILES})
target_link_libraries(NumericalIntegration gflags)
target_link_libraries(NumericalIntegration boost_python)
target_link_libraries(NumericalIntegration python2.7)
