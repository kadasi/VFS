cmake_minimum_required(VERSION 2.8.5)
project(VFS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3 -std=c++11 -g")
set(CMAKE_CXX "clang++")

add_executable(fs _glue.cpp fs.cpp fs-manager.cpp fs-memory.cpp tests.cpp snapshot.cpp)
