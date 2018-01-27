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
    virtual ~Game() {}
    virtual bool operator==(const Game& g) const = 0;
    virtual explicit operator bool() const = 0;
    virtual std::size_t hash() const = 0;
    virtual void do_step(Step& s) = 0;
    virtual void undo_step(Step& s) = 0;
    virtual std::vector<Step> valid_steps() const = 0;
    virtual bool win() const = 0;
};

template<class Step>
using GamePtr = std::shared_ptr<Game<Step>>;

#endif