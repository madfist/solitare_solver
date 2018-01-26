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
    ScorpionGame(Deck&, bool s = false);

    ~ScorpionGame() override {}
    bool operator==(const Game&) const override;
    bool empty() const override;
    GameState state() const override;
    void state(GameState&) override;
    void do_step(ScorpionStep&) override;
    void undo_step(ScorpionStep&) override;
    std::list<ScorpionStep> valid_steps() const override;
    bool win() const override;

    friend std::ostream& operator<<(std::ostream&, const ScorpionGame&);
    friend std::istream& operator>>(std::istream&, ScorpionGame&);
private:
    void deal(Deck& d);

    void do_stock_move();
    void undo_stock_move();
    void do_move_and_upturn(ScorpionStep&);
    void undo_move_and_upturn(ScorpionStep&);

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