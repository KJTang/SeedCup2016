cmake_minimum_required (VERSION 2.8)

project (seedcup)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -std=c++11 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -std=c++11")

aux_source_directory(. DIR_SRCS)
add_executable(SeedCup ${DIR_SRCS})
