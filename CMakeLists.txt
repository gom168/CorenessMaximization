cmake_minimum_required(VERSION 3.10)
project(CoreMaximization)

SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(CoreMaximization main.cpp DataGraph.h DataGraph.cpp Master.h Master.cpp Partition.h Partition.cpp)