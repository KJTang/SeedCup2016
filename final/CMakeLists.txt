cmake_minimum_required (VERSION 2.8)

project(seedcup-finals)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -std=c++11 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -std=c++11")

include_directories(cppjieba/include cppjieba/deps)

file(GLOB SOURCES "src/*.cpp")

add_executable(SeedCup2016.exe ${SOURCES})
