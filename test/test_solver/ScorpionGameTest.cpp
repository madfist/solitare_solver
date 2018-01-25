#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "Card.hpp"
#include "Deck.hpp"

#define private public
#include "ScorpionGame.hpp"
#undef private

TEST(scorpion_game_test, io) {
    Deck deck;
    ScorpionGame game(deck);
    std::cout << game << std::endl;

    std::ifstream f("test/test_solver/data/one_step_win.scorpion.game");
    if (f.fail()) {
        FAIL() << "Cannot open data file\n";
    }
    f >> game;
    f.close();

    EXPECT_EQ(game.piles[0].front(), Card("SA^"));
    EXPECT_TRUE(game.stock.empty());
}

TEST(scorpion_game_test, win) {
    Deck deck;
    ScorpionGame game(deck);

    std::ifstream f("test/test_solver/data/win.scorpion.game");
    if (f.fail()) {
        FAIL() << "Cannot open data file\n";
    }
    f >> game;
    f.close();
    std::cout << game << std::endl;
    EXPECT_TRUE(game.win());
}