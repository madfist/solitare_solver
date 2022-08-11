include_directories("${cxxopts_SOURCE_DIR}/include")

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
    VersatileMask.hpp
    ParallelSolver.hpp
    PrettyPrint.hpp
    Taboo.hpp Taboo.cpp
    Trace.hpp Trace.cpp)

add_library(solver STATIC ${LIB_SRCS})
target_include_directories(solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (UPPER_BUILD_TYPE MATCHES "DEBUG")
    add_library(solver_cov STATIC ${LIB_SRCS})
    target_include_directories(solver_cov PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_options(solver_cov PUBLIC -fprofile-arcs -ftest-coverage)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

add_executable(solitare_solver main.cpp)

target_link_libraries(solitare_solver solver pthread)
