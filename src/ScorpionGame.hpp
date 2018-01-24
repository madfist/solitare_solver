#ifndef SOLITARE_SOLVER_SCORPION_GAME_HEADER
#define SOLITARE_SOLVER_SCORPION_GAME_HEADER

#include <list>

#include "Game.hpp"
#include "Deck.hpp"

struct ScorpionStep {
    bool is_stock_move();

    unsigned from;
    unsigned to;
    Card card;
    bool turnedup;
};

class ScorpionGame : public Game<ScorpionStep> {
public:
    ScorpionGame();
    ScorpionGame(Deck& d);

    ~ScorpionGame() override {}
    bool operator==(const Game& g) override;
    void do_step(ScorpionStep& s) override;
    void undo_step(ScorpionStep& s) override;
    std::list<ScorpionStep> valid_steps() override;
    bool end() override;

    friend std::ostream& operator<<(std::ostream& os, const ScorpionGame& g);
    friend std::istream& operator>>(std::istream& is, ScorpionGame& g);
private:
    void deal(Deck& d);

    void do_stock_move();
    void undo_stock_move();
    void do_move_and_upturn(ScorpionStep& s);
    void undo_move_and_upturn(ScorpionStep& s);

    std::vector<Pile> piles;
    Pile stock;
};

#endif