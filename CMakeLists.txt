# 用于指定需要的CMake最低版本
cmake_minimum_required(VERSION 3.16.3)
project(relay)
# 用于指明构建目标的include目录
include_directories(./include)
add_executable(server ./src/server_epoll.cpp ./src/server.cpp ./src/server_map.cpp ./src/lib.cpp)
add_executable(client ./src/client_epoll.cpp ./src/client.cpp ./src/client_map.cpp ./src/lib.cpp)
# 表示在编译时加上-pg选项
add_definitions("-Wall -pg")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# 表示在链接时加上-pg选项
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

