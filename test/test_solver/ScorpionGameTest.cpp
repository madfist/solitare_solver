#include <iostream>

#include "gtest/gtest.h"
#include "Card.hpp"
#include "Deck.hpp"
#include "ScorpionGame.hpp"

TEST(scorpion_game_test, create) {
    Deck deck;
    ScorpionGame sg(deck);
    std::cout << sg << std::endl;
}