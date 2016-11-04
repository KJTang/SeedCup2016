cmake_minimum_required (VERSION 2.8)

project (seedcup)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -std=c++11 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -std=c++11")


set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

include_directories(${COMMON_INCLUDES})
file(GLOB SOURCES "src/*.cpp")

add_executable(SeedCup2016 ${SOURCES})

add_library(seedcup STATIC ${SOURCES})

add_subdirectory(test)
