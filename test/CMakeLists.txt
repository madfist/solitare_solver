set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_EXTENSIONS 1)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(test_solver)