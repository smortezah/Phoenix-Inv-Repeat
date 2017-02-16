cmake_minimum_required(VERSION 2.8.0)
project(phoenix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES src/phoenix.cpp src/messages.cpp src/functions.cpp src/FCM.cpp)
add_executable(phoenix ${SOURCE_FILES})