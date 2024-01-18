project(task1)
cmake_minimum_required(VERSION 3.10)

FILE(GLOB SOURCES src/*)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(taks1 ${SOURCES})