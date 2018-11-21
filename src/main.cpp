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
#include "PrettyPrint.hpp"
#include "Taboo.hpp"

template <class Game>
std::shared_ptr<Game> load_game(const std::string& filename) {
    auto game = std::make_shared<Game>();
    std::ifstream in(filename);
    if (in.fail()) {
        return game;
    }
    in >> *game;
    in.close();
    return game;
}

template<class Game>
void solve_game(const std::string& filename, bool parallel, bool filter) {
    auto game = std::make_shared<Game>();
    if (!filename.empty()) {
        game = load_game<Game>(filename);
    } else {
        Deck deck;
        game.reset(new Game(deck, true));
    }

    Log(Log::INFO) << *game;

    if (!parallel) {
        auto taboo = std::make_shared<Taboo>();
        Solver<typename Game::step_type> solver(game, taboo, {filter});
        auto solution = solver.solve();

        Log(Log::INFO) << solution;
        if (solution) {
            Log(Log::INFO) << "YEAH";
        }
    } else {
        ParallelSolver<Game> solver(game, {filter});
        auto solution = solver.solve();

        Log(Log::INFO) << solution;
        if (solution) {
            Log(Log::INFO) << "YEAH";
        }
    }
}

template<class Game>
void run_steps(const std::string& filename, const std::string& steps_filename) {
    auto game = load_game<Game>(filename);
    std::vector<typename Game::step_type> steps;
    if (steps_filename.empty()) {
        Log(Log::ERROR) << "Provide steps filename";
        return;
    }
    std::ifstream in(steps_filename);
    if (in.good()) {
        in >> steps;
    }

    Log(Log::INFO) << *game;

    struct PrettyPrint pp;

    for (auto s = steps.begin(); s != steps.end(); ++s) {
        Log(Log::INFO) << "\t\t\t------------\t step: " << pp << *s;
        Log(Log::INFO) << "*** " << pp << game->valid_steps();
        game->do_step(*s);
        Log(Log::INFO) << "--------------------------------------------------------\n" << *game;
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
        ("s,steps", "steps to run", cxxopts::value<std::string>())
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
            if (opts.count("steps")) {
                run_steps<ScorpionGame>(filename, opts["steps"].as<std::string>());
            } else {
                solve_game<ScorpionGame>(filename, opts.count("parallel"), opts.count("filter"));
            }
        } else if (opts.count("game") && opts["game"].as<std::string>() == "klondike") {
            if (opts.count("steps")) {
                run_steps<KlondikeGame>(filename, opts["steps"].as<std::string>());
            } else {
                solve_game<KlondikeGame>(filename, opts.count("parallel"), opts.count("filter"));
            }
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