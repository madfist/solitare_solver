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
    shared_mutex.hpp shared_mutex.cpp
    PrettyPrint.hpp
    Taboo.hpp Taboo.cpp)

add_library(libsolver STATIC ${LIB_SRCS})

target_include_directories(libsolver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(solitare_solver main.cpp)

target_link_libraries(solitare_solver libsolver pthread)