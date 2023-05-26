cmake_minimum_required(VERSION 3.12)
project(alex)

set(CMAKE_CXX_STANDARD 14)

# Define the macro ‘DEBUG' in the debug mode
if(CMAKE_BUILD_TYPE STREQUAL Debug)        
    ADD_DEFINITIONS(-DDEBUG)               
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "/O2 /arch:AVX2 /W1 /EHsc")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "-O3 -xHost")
else()
    # clang and gcc
    set(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -Wextra -fopenmp")
endif()

include_directories(src/core)

add_executable(el2kv src/utils/el2kv_converter.cpp)
add_executable(example src/examples/main.cpp)
add_executable(graph src/examples/graph.cpp)
add_executable(benchmark src/benchmark/main.cpp)
add_executable(stx_graph_test src/stx-btree/stx_graph_test.cpp)

#set(DOCTEST_DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/doctest)
#file(DOWNLOAD
#    https://raw.githubusercontent.com/onqtam/doctest/2.4.6/doctest/doctest.h
#    ${DOCTEST_DOWNLOAD_DIR}/doctest.h
#    EXPECTED_HASH SHA1=da39a3ee5e6b4b0d3255bfef95601890afd80709
#)
#
#add_executable(test_alex test/unittest_main.cpp)
#target_include_directories(test_alex PRIVATE ${DOCTEST_DOWNLOAD_DIR})
#
#enable_testing()
#add_test(test_alex test_alex)