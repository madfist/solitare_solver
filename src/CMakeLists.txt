set(LIB_SRCS
    Card.hpp Card.cpp
    Deck.hpp Deck.cpp
    Game.hpp
    ScorpionGame.hpp ScorpionGame.cpp
    Rules.hpp Rules.cpp
    Solver.hpp)

add_library(libsolver STATIC ${LIB_SRCS})

target_include_directories(libsolver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(solitare_solver main.cpp)

target_link_libraries(solitare_solver libsolver)