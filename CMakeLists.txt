cmake_minimum_required(VERSION 3.10)
project(garbage)

set(CMAKE_CXX_STANDARD 11)

add_executable(garbage src/main.cpp src/HashMap.cpp)