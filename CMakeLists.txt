cmake_minimum_required(VERSION 3.11)

project(solitare_solver)

include(FetchContent)


FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY      https://github.com/jarro2783/cxxopts.git
    GIT_TAG             v3.0.0
    UPDATE_DISCONNECTED 1
)

FetchContent_Declare(
    googletest
	GIT_REPOSITORY      https://github.com/google/googletest.git
	GIT_TAG             main
    UPDATE_DISCONNECTED 1
)

FetchContent_MakeAvailable(cxxopts googletest)

include(CTest)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake @ONLY)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(DOXYGEN_DOCS ${CMAKE_CURRENT_BINARY_DIR}/docs)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen configured")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

add_subdirectory(src)
add_subdirectory(test)
