cmake_minimum_required(VERSION 3.27)
project(cs3340)

set(CMAKE_CXX_STANDARD 14)

include_directories(assignment3)

add_executable(cs3340
        assignment3/Main.cpp
)
