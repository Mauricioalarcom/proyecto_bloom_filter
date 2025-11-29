cmake_minimum_required(VERSION 3.17)
project(bloom_filter)

set(CMAKE_CXX_STANDARD 17)

add_executable(bloom_filter src/main.cpp src/BloomFilter.cpp src/BloomFilter.h)
