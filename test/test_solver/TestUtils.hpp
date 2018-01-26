#ifndef SOLITARE_SOLVER_TEST_UTILS_HEADER
#define SOLITARE_SOLVER_TEST_UTILS_HEADER

#include "Game.hpp"

namespace test_solver {

template <class G>
std::shared_ptr<G> load_game(const std::string& filename) {
    auto game = std::make_shared<G>();
    std::ifstream f("data/" + filename);
    if (f.fail()) {
        return game;
    }
    f >> *game;
    f.close();
    return game;
}

template <class G>
std::shared_ptr<G> load_game(const char *filename) {
    return load_game<G>(std::string(filename));
}

}

#endif