#include <memory>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"
#include "Solver.hpp"
#include "ScorpionGame.hpp"
#include "TestUtils.hpp"

TEST(solver_test, scorpion_solve_one_step) {
    auto game = test_solver::load_game<ScorpionGame>("one_step_win.scorpion.game");
    if (! *game)
        FAIL() << "Cannot load game data" << std::endl;

    Solver<ScorpionStep> solver(game);
    Solution<ScorpionStep> solution = solver.solve();

    EXPECT_TRUE(game->win());
}