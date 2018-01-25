#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "Card.hpp"
#include "Deck.hpp"
#include "ScorpionGame.hpp"

TEST(scorpion_game_test, load_from_file) {
    Deck deck;
    ScorpionGame game(deck);

    std::ifstream f("data/one_step_win.scorpion.game");
    f >> game;
    f.close();

    std::cout << game << std::endl;
}