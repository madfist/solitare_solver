#include <memory>
#include <iostream>
#include <fstream>

#include "Card.hpp"
#include "Deck.hpp"
#include "ScorpionGame.hpp"
#include "Solver.hpp"

int main(int argc, char *argv[]) {
    Deck deck;
    auto game = std::make_shared<ScorpionGame>(deck, true);
    if (argc > 1) {
        std::ifstream in(argv[1]);
        if (in.good())
            in >> *game;
        in.close();
    }
    
    std::cout << *game << std::endl;
    
    Solver<ScorpionStep> solver(game);
    auto solution = solver.solve();
    
    std::cout << solution << std::endl;
    if (solution) {
        std::cout << "YEAH" << std::endl;
    }
    return 0;
}