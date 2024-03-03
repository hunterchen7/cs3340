cmake_minimum_required(VERSION 3.27)
project(cs3340)

set(CMAKE_CXX_STANDARD 14)

include_directories(assignment2)

add_executable(cs3340
        assignment2/DisjointSet.cpp
        assignment2/DisjointSet.h
        assignment2/main.cpp)
