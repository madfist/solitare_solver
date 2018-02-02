#ifndef SOLITARE_SOLVER_SCORPION_GAME_HEADER
#define SOLITARE_SOLVER_SCORPION_GAME_HEADER

#include "Game.hpp"
#include "Deck.hpp"
#include "Rules.hpp"
#include "ScorpionStep.hpp"

class ScorpionGame : public Game<ScorpionStep> {
public:
    ScorpionGame();
    ScorpionGame(const GameState&);
    ScorpionGame(Deck&, bool s = false);

    ~ScorpionGame() override {}
    bool operator==(const Game&) const override;
    explicit operator bool() const override;
    std::size_t hash() const override;
    void do_step(ScorpionStep&) override;
    void undo_step(ScorpionStep&) override;
    std::vector<ScorpionStep> valid_steps() const override;
    bool win() const override;
    bool sanity() const override;

    friend std::ostream& operator<<(std::ostream&, const ScorpionGame&);
    friend std::istream& operator>>(std::istream&, ScorpionGame&);

private:
    void do_stock_move();
    void undo_stock_move();
    void do_move_and_upturn(ScorpionStep&);
    void undo_move_and_upturn(ScorpionStep&);

    bool is_four_pile_all_ace() const;
    bool pile_empty(unsigned) const;
    unsigned pile_size(unsigned) const;
    unsigned pile_bottom(unsigned) const;
    unsigned pile_top(unsigned) const;
    void move_cards_backward(unsigned, unsigned, unsigned, unsigned);
    void move_cards_forward(unsigned, unsigned, unsigned, unsigned);
    bool deadlock() const;
    unsigned find_card(const CardCode&) const;
    unsigned locked_down_turned() const;

    GameState state;
    Rules rules;
};

#endif