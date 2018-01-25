#ifndef SOLITARE_SOLVER_SCORPION_GAME_HEADER
#define SOLITARE_SOLVER_SCORPION_GAME_HEADER

#include <list>
#include <algorithm>

#include "Game.hpp"
#include "Deck.hpp"
#include "Rules.hpp"

struct ScorpionStep {
    ScorpionStep(unsigned f, unsigned t, const Card& c, bool u = false);

    bool is_stock_move();

    unsigned from;
    unsigned to;
    Card card;
    bool turnedup;
};

std::ostream& operator<<(std::ostream&, const ScorpionStep&);

class ScorpionGame : public Game<ScorpionStep> {
public:
    ScorpionGame();
    ScorpionGame(Deck& d);

    ~ScorpionGame() override {}
    bool operator==(const Game& g) override;
    void do_step(ScorpionStep& s) override;
    void undo_step(ScorpionStep& s) override;
    std::list<ScorpionStep> valid_steps() override;
    bool win() override;

    friend std::ostream& operator<<(std::ostream& os, const ScorpionGame& g);
    friend std::istream& operator>>(std::istream& is, ScorpionGame& g);
private:
    void deal(Deck& d);

    void do_stock_move();
    void undo_stock_move();
    void do_move_and_upturn(ScorpionStep& s);
    void undo_move_and_upturn(ScorpionStep& s);

    bool is_four_pile_all_ace() const;

    template<class F>
    void for_all_piles(F f) {
        for (unsigned i = 0; i < piles.size(); ++i) {
            f(i, piles[i]);
        }
    }
    template<class F>
    void for_all_piles(F f) const {
        for (unsigned i = 0; i < piles.size(); ++i) {
            f(i, piles[i]);
        }
    }

    std::vector<Pile> piles;
    Pile stock;
    Rules rules;
};

#endif