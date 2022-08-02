#include <memory>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"
#include "Solver.hpp"
#include "Taboo.hpp"
#include "ScorpionGame.hpp"
#include "TestUtils.hpp"

TEST(solver_test, scorpion_solve_one_step) {
    auto game = test_solver::load_game<ScorpionGame>("one_step_win.scorpion.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;

    std::shared_ptr<Taboo> taboo(new Taboo());
    Solver<ScorpionStep> solver(game, taboo);
    Solution<ScorpionStep> solution = solver.solve();

    EXPECT_TRUE(game->win());
}

TEST(solver_test, scorpion_solve_insane) {
    auto game = test_solver::load_game<ScorpionGame>("insane.scorpion.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;

    std::shared_ptr<Taboo> taboo(new Taboo());
    Solver<ScorpionStep> solver(game, taboo);
    Solution<ScorpionStep> solution = solver.solve();

    EXPECT_FALSE(game->win());
}

TEST(solver_test, scorpion_win_with_taboo) {
    auto game = test_solver::load_game<ScorpionGame>("24level_solution.scorpion.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;

    std::shared_ptr<Taboo> taboo(new Taboo());
    Solver<ScorpionStep> solver(game, taboo);
    Solution<ScorpionStep> solution = solver.solve();

    EXPECT_TRUE(game->win());
    EXPECT_GT(taboo->size(), 0);
}

TEST(solver_test, scorpion_no_solution) {
    GTEST_SKIP() << "This is skipped until fixed";
    auto game = test_solver::load_game<ScorpionGame>("68k_no_solution.scorpion.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;

    std::shared_ptr<Taboo> taboo(new Taboo());
    Solver<ScorpionStep> solver(game, taboo);
    Solution<ScorpionStep> solution = solver.solve();
    std::cout << solution;

    EXPECT_FALSE(game->win());
    EXPECT_TRUE(game->sanity());
    EXPECT_GT(taboo->size(), 0);
}
