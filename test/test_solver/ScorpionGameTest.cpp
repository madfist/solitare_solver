#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Card.hpp"
#include "Deck.hpp"
#include "TestUtils.hpp"

#define private public
#include "ScorpionGame.hpp"
#undef private

TEST(scorpion_game_test, empty) {
    ScorpionGame game;

    EXPECT_TRUE(game.empty());
}

TEST(scorpion_game_test, state) {
    ScorpionGame game;
    game.piles[0].push_front(Card("CA_"));
    game.piles[1].push_front(Card("D3^"));

    GameState state = game.state();

    EXPECT_THAT(state, testing::ElementsAreArray({
        Card::card_separator().get(), Card::card_separator().get(), Card::card_separator().get(),
        Card::card_separator().get(), Card::card_separator().get(), Card::card_separator().get(),
        Card("D3^").get(), Card::card_separator().get(), Card("CA_").get()
    }));

    state.push_front(Card("H4_").get());
    game.state(state);

    EXPECT_EQ(game.stock.front(), Card("H4_"));
}

TEST(scorpion_game_test, io) {
    auto game = test_solver::load_game<ScorpionGame>("one_step_win.scorpion.game");
    if (game->empty())
        FAIL() << "Cannot load game data" << std::endl;

    EXPECT_EQ(game->piles[0].front(), Card("SA^"));
    EXPECT_TRUE(game->stock.empty());
}

TEST(scorpion_game_test, valid_steps) {
    auto game = test_solver::load_game<ScorpionGame>("one_step_win.scorpion.game");
    if (game->empty())
        FAIL() << "Cannot load game data" << std::endl;

    auto steps = game->valid_steps();
    EXPECT_EQ(steps.size(), 1);
}

TEST(scorpion_game_test, win) {
    auto game = test_solver::load_game<ScorpionGame>("win.scorpion.game");
    if (game->empty())
        FAIL() << "Cannot load game data" << std::endl;

    EXPECT_TRUE(game->win());
}