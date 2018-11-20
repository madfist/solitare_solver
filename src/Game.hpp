#ifndef SOLITARE_SOLVER_GAME_HEADER
#define SOLITARE_SOLVER_GAME_HEADER

#include <memory>
#include <vector>
#include <algorithm>

#include "Card.hpp"

typedef std::vector<CardCode> GameState;

std::ostream& operator<<(std::ostream&, const GameState&);

template<class Step>
class Game {
public:
    typedef Step step_type;

    virtual ~Game() {}
    virtual bool operator==(const Game&) const = 0; ///< Compare games on state level
    virtual explicit operator bool() const = 0; ///< Tells if the game is set up
    virtual std::size_t hash() const = 0; ///< Hash value for the game state (CRC32/CRC64)
    virtual void do_step(const Step&) = 0; ///< Take a step
    virtual void do_steps(const std::vector<Step>& steps) {
        std::for_each(steps.begin(), steps.end(), [=](const Step& s) {
            do_step(s);
        });
    }
    virtual void undo_step(const Step&) = 0; ///< Undo a step
    virtual std::vector<Step> valid_steps() const = 0; ///< Gather the possible steps from the current state
    virtual bool win() const = 0; ///< Tells if the game is in a win state
    virtual bool sanity() const = 0; ///< Tells if it even worth trying to solve the game
    virtual std::ostream& print(std::ostream&) const = 0;
    virtual std::istream& read(std::istream&) = 0;
};

template<class Step>
using GamePtr = std::shared_ptr<Game<Step>>;

template<class Step>
std::ostream& operator<<(std::ostream& os, const Game<Step>& g) {
    return g.print(os);
}

template<class Step>
std::istream& operator>>(std::istream& is, const Game<Step>& g) {
    return g.read(is);
}

#endif