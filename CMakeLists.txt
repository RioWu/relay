#用于指定需要的CMake最低版本
cmake_minimum_required(VERSION 3.16.3)
project(relay)
# 用于指明构建目标的include目录
include_directories(./include)
add_executable(server ./src/server_epoll.cpp ./src/server_map.cpp ./src/server.cpp ./src/lib.cpp)
add_executable(client ./src/client_epoll.cpp ./src/client.cpp ./src/client_map.cpp ./src/lib.cpp)
add_definitions("-Wall -g")
