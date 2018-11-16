#ifndef SOLITARE_SOLVER_KLONDIKE_GAME_HEADER
#define SOLITARE_SOLVER_KLONDIKE_GAME_HEADER

#include "Game.hpp"
#include "Deck.hpp"
#include "KlondikeStep.hpp"
#include "Rules.hpp"
#include "SingleVectorGameState.hpp"

typedef unsigned pile;

class KlondikeGame : public Game<KlondikeStep> {
public:
    KlondikeGame();
    KlondikeGame(Deck&, bool shuffle = false);

    ~KlondikeGame() override {}
    bool operator==(const Game&) const override;
    explicit operator bool() const override;
    std::size_t hash() const override;
    void do_step(const KlondikeStep&) override;
    void undo_step(const KlondikeStep&) override;
    std::vector<KlondikeStep> valid_steps() const override;
    bool win() const override;
    bool sanity() const override;
    std::ostream& print(std::ostream&) const override;
    std::istream& read(std::istream&) override;

private:
    SingleVectorGameState state;
    Rules pile_rules;
    Rules foundation_rules;
};

std::ostream& operator<<(std::ostream&, const KlondikeGame&);
std::istream& operator>>(std::istream&, KlondikeGame&);

#endif