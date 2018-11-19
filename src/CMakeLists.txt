cmake_minimum_required(VERSION 2.8.2)

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(../DownloadProject.cmake)
download_project(PROJ                cxxopts
                 GIT_REPOSITORY      https://github.com/jarro2783/cxxopts.git
                 GIT_TAG             v2.1.1
                 PREFIX              external
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${cxxopts_SOURCE_DIR} ${cxxopts_BINARY_DIR})

# if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${cxxopts_SOURCE_DIR}/include")
# endif()

set(LIB_SRCS
    Card.hpp Card.cpp
    Deck.hpp Deck.cpp
    Game.hpp Game.cpp
    ScorpionStep.hpp ScorpionStep.cpp
    ScorpionGame.hpp ScorpionGame.cpp
    Rules.hpp Rules.cpp
    RuledCard.hpp RuledCard.cpp
    Solution.hpp
    Solver.hpp
    Log.hpp Log.cpp
    crc.hpp crc.cpp
    SingleVectorGameState.hpp SingleVectorGameState.cpp
    SingleVectorPileStep.hpp SingleVectorPileStep.cpp
    KlondikeGame.hpp KlondikeGame.cpp
    KlondikeStep.hpp KlondikeStep.cpp
    VersatileMask.hpp)

add_library(libsolver STATIC ${LIB_SRCS})

target_include_directories(libsolver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(solitare_solver main.cpp)

target_link_libraries(solitare_solver libsolver)