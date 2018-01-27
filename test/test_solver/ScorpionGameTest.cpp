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

    EXPECT_TRUE(!game);
}

TEST(scorpion_game_test, from_deck) {
    Deck deck;
    ScorpionGame game(deck);

    GameState positions(game.state.begin(), game.state.begin()+7);
    EXPECT_THAT(positions, testing::ElementsAre(14, 21, 28, 35, 42, 49, 56));

    GameState firstthree(game.state.begin()+7, game.state.begin()+10);
    EXPECT_THAT(firstthree, testing::ElementsAre(Card("DK_").get(), Card("DQ_").get(), Card("DJ^").get()));
}

TEST(scorpion_game_test, io) {
    auto game = test_solver::load_game<ScorpionGame>("one_step_win.scorpion.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;

    GameState positions(game->state.begin(), game->state.begin()+7);
    EXPECT_THAT(positions, testing::ElementsAre(8, 20, 33, 46, 59, 59, 59));

    GameState firstthree(game->state.begin()+7, game->state.begin()+10);
    EXPECT_THAT(firstthree, testing::ElementsAre(Card("SA^").get(), Card("SK^").get(), Card("SQ^").get()));

    // std::cout << "game:" << std::endl << *game << std::endl;
}

TEST(scorpion_game_test, valid_steps) {
    auto game = test_solver::load_game<ScorpionGame>("one_step_win.scorpion.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;
    // std::cout << game->state << std::endl;

    auto steps = game->valid_steps();
    EXPECT_EQ(steps.size(), 1);
    EXPECT_EQ(steps.front().card_code(), Card("SA^").get());
    EXPECT_EQ(steps.front().pile_from(), 0);
    EXPECT_EQ(steps.front().pile_to(), 1);
    // std::cout << *game << std::endl;

    // std::for_each(steps.begin(), steps.end(), [] (const ScorpionStep& s) {
        // std::cout << s << std::endl;
    // });
    auto hash = game->hash();

    EXPECT_TRUE(!game->win());
    game->do_move_and_upturn(steps.front());
    EXPECT_TRUE(game->win());
    game->undo_move_and_upturn(steps.front());
    EXPECT_TRUE(!game->win());
    EXPECT_EQ(hash, game->hash());

    ScorpionStep step(Card("SA^").get(), 0, 5, 0, 0);
    game->do_move_and_upturn(step);
    steps = game->valid_steps();

    hash = game->hash();
    game->do_move_and_upturn(steps.front());
    EXPECT_TRUE(game->win());
    game->undo_move_and_upturn(steps.front());
    EXPECT_TRUE(!game->win());
    EXPECT_EQ(hash, game->hash());

    // std::cout << *game << std::endl;
}

TEST(scorpion_game_test, stock_move) {
    auto game = test_solver::load_game<ScorpionGame>("one_stock_step_win.scorpion.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;
    // std::cout << *game << std::endl;

    auto hash = game->hash();
    auto steps = game->valid_steps();
    EXPECT_TRUE(!game->win());
    game->do_step(steps.front());
    EXPECT_TRUE(game->win());
    // std::cout << *game << std::endl;
    game->undo_step(steps.front());
    EXPECT_TRUE(!game->win());
    EXPECT_EQ(hash, game->hash());
    // std::cout << *game << std::endl;
}

TEST(scorpion_game_test, win) {
    auto game = test_solver::load_game<ScorpionGame>("win.scorpion.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;

    EXPECT_TRUE(game->win());
}