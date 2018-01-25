#include <memory>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"
#include "Solver.hpp"
#include "ScorpionGame.hpp"

TEST(solver_test, scorpion_solve_one_step) {
    auto game = std::make_shared<ScorpionGame>();

    std::ifstream f("test/test_solver/data/one_step_win.scorpion.game");
    if (f.fail()) {
        FAIL() << "Cannot open data file\n";
    }
    f >> *game;
    f.close();

    Solver<ScorpionStep> solver(game);
    // Solution<ScorpionStep> solution = solver.solve();
}