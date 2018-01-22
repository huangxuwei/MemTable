cmake_minimum_required(VERSION 2.8.3)
project(memtable)

include_directories()

link_directories()

file(GLOB SOURCES src/*.cpp)

add_definitions(-std=c++11)
add_executable(memtable ${SOURCES})
