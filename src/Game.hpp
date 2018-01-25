#ifndef SOLITARE_SOLVER_GAME_HEADER
#define SOLITARE_SOLVER_GAME_HEADER

#include <list>

template<typename Step>
class Game {
public:
    virtual ~Game() {}
    virtual bool operator==(const Game& g) = 0;
    virtual void do_step(Step& s) = 0;
    virtual void undo_step(Step& s) = 0;
    virtual std::list<Step> valid_steps() = 0;
    virtual bool win() = 0;
};

#endif