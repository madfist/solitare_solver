#include <memory>
#include <iostream>
#include <fstream>

#include "Card.hpp"
#include "Deck.hpp"
#include "ScorpionGame.hpp"
#include "Solver.hpp"
#include "Log.hpp"

int main(int argc, char *argv[]) {
    Log::set_level(Log::INFO);
    Deck deck;
    auto game = std::make_shared<ScorpionGame>(deck, true);
    if (argc > 1) {
        std::cout << "Reading: " << argv[1] << std::endl;
        std::ifstream in(argv[1]);
        if (in.good())
            in >> *game;
        in.close();
    }

    Log(Log::INFO) << *game;

    Solver<ScorpionStep> solver(game);
    auto solution = solver.solve();

    Log(Log::INFO) << solution;
    if (solution) {
        Log(Log::INFO) << "YEAH";
    }
    return 0;
}