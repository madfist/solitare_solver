cmake_minimum_required(VERSION 3.6)

project(solitare_solver)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

add_subdirectory(src)
add_subdirectory(test)