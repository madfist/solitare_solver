cmake_minimum_required(VERSION 3.10)

project(solitare_solver)

include(CTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(test)