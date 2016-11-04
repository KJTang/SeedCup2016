cmake_minimum_required (VERSION 2.8)

project (seedcup)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -std=c++11 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -std=c++11")

add_subdirectory(googletest/googletest)

set(TEST_INCLUDES ../include
                  ./testcase
                  #${GTEST_INCLUDE_DIRS}
                )
set(TEST_SOURCE unittest.cpp)

include_directories(${TEST_INCLUDES})

file(COPY testcase/ DESTINATION ./Debug)

add_executable(SeedCup_test ${TEST_SOURCE})

target_link_libraries(SeedCup_test gtest seedcup)
