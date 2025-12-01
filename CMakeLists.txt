cmake_minimum_required(VERSION 3.17)

project(bloom_filter)

set(CMAKE_CXX_STANDARD 17)


add_executable(bloom_filter src/main.cpp src/BloomFilter.cpp src/BloomFilter.h)

# Backend server
add_executable(bloom_filter_server src/server.cpp src/BloomFilter.cpp src/BloomFilter.h)
target_include_directories(bloom_filter_server PRIVATE ${CMAKE_SOURCE_DIR}/external)
