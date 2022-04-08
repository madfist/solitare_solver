#include <iostream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "TestUtils.hpp"

#define private public
#include "KlondikeGame.hpp"
#undef private

TEST(klondike_game_test, empty) {
    KlondikeGame game;
    EXPECT_TRUE(!game);
}

TEST(klondike_game_test, from_deck) {
    Deck deck;
    KlondikeGame game(deck);
    // std::cout << game << std::endl;
    // std::cout << game.state << std::endl;

    GameState positions(game.state.state.begin(), game.state.state.begin()+11);
    EXPECT_THAT(positions, testing::ElementsAre(12, 14, 17, 21, 26, 32, 39, 39, 39, 39, 39));

    GameState firstthree(game.state.state.begin()+11, game.state.state.begin()+14);
    EXPECT_THAT(firstthree, testing::ElementsAre(Card("DK^").get(), Card("DQ_").get(), Card("DJ^").get()));

    GameState lastthree(game.state.state.end()-3, game.state.state.end());
    EXPECT_THAT(lastthree, testing::ElementsAre(Card("S3^").get(), Card("S2^").get(), Card("SA^").get()));
}

TEST(klondike_game_test, win) {
    auto game = test_solver::load_game<KlondikeGame>("win.klondike.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;
    // std::cout << *game << std::endl;

    EXPECT_TRUE(game->win());

    auto game2 = test_solver::load_game<KlondikeGame>("win2.klondike.game");
    if (! *game2)
        FAIL() << "Cannot load game data" << std::endl;
    // std::cout << *game << std::endl;

    EXPECT_TRUE(game2->win());
}

TEST(klondike_game_test, valid_steps) {
    auto game = test_solver::load_game<KlondikeGame>("win3.klondike.game");
    // std::cout << *game << std::endl;
    auto steps = game->valid_steps();
    // std::cout << steps << std::endl;
    game->do_step(steps.front());
    // std::cout << *game << std::endl;
    game->undo_step(steps.front());
    // std::cout << *game << std::endl;
}