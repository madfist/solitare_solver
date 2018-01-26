#include <iostream>

#include "Game.hpp"

std::ostream& operator<<(std::ostream& os, const GameState& gs) {
    for (auto cc = gs.begin(); cc != gs.end(); ++cc) {
        os << "[" << *cc << "]";
    }
    return os;
}