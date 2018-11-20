#include <memory>
#include <iostream>
#include <fstream>

#include <cxxopts.hpp>

#include "Card.hpp"
#include "Deck.hpp"
#include "ScorpionGame.hpp"
#include "KlondikeGame.hpp"
#include "Solver.hpp"
#include "ParallelSolver.hpp"
#include "Log.hpp"

template<class Game>
void solve_game(const std::string& filename, bool parallel, bool filter) {
    auto game = std::make_shared<Game>();
    if (!filename.empty()) {
        Log(Log::INFO) << "Reading: " << filename << std::endl;
        std::ifstream in(filename.c_str());
        if (in.good())
            in >> *game;
        else {
            Log(Log::ERROR) << "Cannot open input file " << filename << std::endl;
            return;
        }
        in.close();
    } else {
        Deck deck;
        game.reset(new Game(deck, true));
    }

    Log(Log::INFO) << *game;

    if (!parallel) {
        Solver<typename Game::step_type> solver(game, {filter});
        auto solution = solver.solve();

        Log(Log::INFO) << solution;
        if (solution) {
            Log(Log::INFO) << "YEAH";
        }
    } else {
        ParallelSolver<Game> solver(game);
        solver.solve();
    }
}

int main(int argc, char *argv[]) {
    Log::set_level(Log::WARN);

    cxxopts::Options options("Solitare Solver", "Solve various solitare games");
    options.add_options()
        ("g,game", "solitare game type (scorpion|klondike)", cxxopts::value<std::string>()->default_value("scorpion"))
        ("i,input", "input game state", cxxopts::value<std::string>())
        ("v,verbose", "info messages")
        ("d,debug", "debug messages")
        ("p,parallel", "use parallel solver")
        ("f,filter", "filter valid steps")
        ("h,help", "display this help");

    try {
        auto opts = options.parse(argc, argv);

        if (opts.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (opts.count("verbose")) {
            Log::set_level(Log::INFO);
        }

        if (opts.count("debug")) {
            Log::set_level(Log::DEBUG);
        }

        std::string filename;
        if (opts.count("input"))
            filename = opts["input"].as<std::string>();

        if (opts["game"].as<std::string>() == "scorpion") {
            solve_game<ScorpionGame>(filename, opts.count("parallel"), opts.count("filter"));
        } else if (opts.count("game") && opts["game"].as<std::string>() == "klondike") {
            solve_game<KlondikeGame>(filename, opts.count("parallel"), opts.count("filter"));
        } else {
            Log(Log::ERROR) << "Unknown game type: " << opts["game"].as<std::string>() << std::endl;
            return 1;
        }
    } catch(cxxopts::OptionParseException e) {
        Log(Log::ERROR) << e.what() << std::endl;
        return 1;
    }



    return 0;
}