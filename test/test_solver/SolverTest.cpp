#include <memory>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"
#include "Solver.hpp"
#include "ScorpionGame.hpp"

TEST(solver_test, scorpion_solve_one_step) {
    auto game = std::make_shared<ScorpionGame>();

    std::ifstream f("data/one_step_win.scorpion.game");
    f >> *game;
    f.close();

    std::cout << *game << std::endl;

    Solver<ScorpionStep> solver(game);
    // Solution<ScorpionStep> solution = solver.solve();
}