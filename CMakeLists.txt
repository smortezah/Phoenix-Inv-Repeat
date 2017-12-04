cmake_minimum_required(VERSION 2.8.0)
project(phoenixIR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES src/phoenixIR.cpp src/FCM.cpp src/messages.cpp)
add_executable(phoenixIR ${SOURCE_FILES})
