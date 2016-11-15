cmake_minimum_required(VERSION 3.2)
project(phoenix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES phoenix.cpp messages.cpp functions.cpp)
add_executable(phoenix ${SOURCE_FILES})