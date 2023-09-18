/**
 * Abstract Card Game
 * @file Game.hpp
 * @author Adam Koleszar
 */
#ifndef SOLITARE_SOLVER_GAME_HEADER
#define SOLITARE_SOLVER_GAME_HEADER

#include <memory>
#include <vector>
#include <algorithm>

#include "Card.hpp"
#include "SingleVectorGameState.hpp"

typedef std::vector<CardCode> GameState; ///< State of game represented as ::CardCode vector

std::ostream& operator<<(std::ostream&, const GameState&); ///< Stream output operator

/**
 * Abstract card game
 * @tparam Step step type
 */
template<class Step, class State = SingleVectorGameState>
class Game {
public:
    typedef Step step_type;
    using Hash = StateHash<State>;

    virtual ~Game() {}
    virtual bool operator==(const Game&) const = 0;       ///< Compare games on state level
    virtual explicit operator bool() const = 0;           ///< Tells if the game is set up
    virtual std::size_t hash() const = 0;                 ///< Hash value for the game state (CRC32/CRC64)
    virtual void do_step(const Step&) = 0;                ///< Take a step
    /// Take multiple steps
    virtual void do_steps(const std::vector<Step>& steps) {
        std::for_each(steps.begin(), steps.end(), [this](const Step& s) {
            do_step(s);
        });
    }
    virtual void undo_step(const Step&) = 0;              ///< Undo a step
    virtual std::vector<Step> valid_steps() const = 0;    ///< Gather the possible steps from the current state
    virtual bool win() const = 0;                         ///< Tells if the game is in a win state
    virtual bool sanity() const = 0;                      ///< Tells if it even worth trying to solve the game
    virtual State state() const {
        return state_;
    }
    virtual std::ostream& print(std::ostream&) const = 0; ///< Stream output method
    virtual std::istream& read(std::istream&) = 0;        ///< Stream input method
protected:
    State state_;
};

template<class Step>
using GamePtr = std::shared_ptr<Game<Step>>;

/// Abstract stream output operator
template<class Step>
std::ostream& operator<<(std::ostream& os, const Game<Step>& g) {
    return g.print(os);
}

/// Abstract stream input operator
template<class Step>
std::istream& operator>>(std::istream& is, const Game<Step>& g) {
    return g.read(is);
}

#endif
