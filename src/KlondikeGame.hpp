#ifndef SOLITARE_SOLVER_KLONDIKE_GAME_HEADER
#define SOLITARE_SOLVER_KLONDIKE_GAME_HEADER

#include "Game.hpp"
#include "Deck.hpp"
#include "KlondikeStep.hpp"
#include "Rules.hpp"
#include "SingleVectorGameState.hpp"

class KlondikeGame : public Game<KlondikeStep> {
public:
    KlondikeGame();
    KlondikeGame(Deck&, bool shuffle = false);

    ~KlondikeGame() override {}
    bool operator==(const Game&) const override;
    explicit operator bool() const override;
    std::size_t hash() const override;
    void do_step(KlondikeStep&) override;
    void undo_step(KlondikeStep&) override;
    std::vector<KlondikeStep> valid_steps() const override;
    bool win() const override;
    bool sanity() const override;

    friend std::ostream& operator<<(std::ostream&, const KlondikeGame&);
    friend std::istream& operator>>(std::istream&, KlondikeGame&);

private:
    SingleVectorGameState state;
    Rules pile_rules;
    Rules foundation_rules;
};

#endif