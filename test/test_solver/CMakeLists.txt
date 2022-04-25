file(GLOB SRCS *.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

add_executable(solver_test ${SRCS})

target_link_libraries(solver_test libsolver_cov gtest gmock_main gcov)

add_test(NAME solver_test
         COMMAND solver_test
         WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
