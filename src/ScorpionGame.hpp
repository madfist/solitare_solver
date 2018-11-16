#ifndef SOLITARE_SOLVER_SCORPION_GAME_HEADER
#define SOLITARE_SOLVER_SCORPION_GAME_HEADER

#include "Game.hpp"
#include "Deck.hpp"
#include "Rules.hpp"
#include "ScorpionStep.hpp"
#include "SingleVectorGameState.hpp"

class ScorpionGame : public Game<ScorpionStep> {
public:
    ScorpionGame();
    ScorpionGame(Deck&, bool shuffle = false);

    ~ScorpionGame() override {}
    bool operator==(const Game&) const override;
    explicit operator bool() const override;
    std::size_t hash() const override;
    void do_step(const ScorpionStep&) override;
    void undo_step(const ScorpionStep&) override;
    std::vector<ScorpionStep> valid_steps() const override;
    bool win() const override;
    bool sanity() const override;
    std::ostream& print(std::ostream&) const override;
    std::istream& read(std::istream&) override;

private:
    void do_stock_move();
    void undo_stock_move();
    void do_move_and_upturn(const ScorpionStep&);
    void undo_move_and_upturn(const ScorpionStep&);

    bool is_four_pile_all_ace() const;
    bool deadlock() const;
    unsigned locked_down_turned() const;

    SingleVectorGameState state;
    Rules rules;
};

std::ostream& operator<<(std::ostream&, const ScorpionGame&);
std::istream& operator>>(std::istream&, ScorpionGame&);

#endif