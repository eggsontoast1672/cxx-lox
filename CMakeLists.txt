cmake_minimum_required(VERSION 3.5)
project(Lox)

add_executable(lox src/chunk.cpp src/debug.cpp src/main.cpp)
target_include_directories(lox PRIVATE include)
