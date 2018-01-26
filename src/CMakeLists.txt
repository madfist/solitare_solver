set(LIB_SRCS
    Card.hpp Card.cpp
    Deck.hpp Deck.cpp
    Game.hpp Game.cpp
    ScorpionGame.hpp ScorpionGame.cpp
    Rules.hpp Rules.cpp
    Solver.hpp
    crc32.h crc32.c)

add_library(libsolver STATIC ${LIB_SRCS})

target_include_directories(libsolver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(solitare_solver main.cpp)

target_link_libraries(solitare_solver libsolver)