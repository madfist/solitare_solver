#ifndef SOLITARE_SOLVER_KLONDIKE_GAME_HEADER
#define SOLITARE_SOLVER_KLONDIKE_GAME_HEADER

#include "Game.hpp"
#include "Deck.hpp"
#include "KlondikeStep.hpp"
#include "Rules.hpp"

typedef unsigned pile; ///< A pile is represented as an `unsigned`

/**
 * @brief Klondike Game
 * @details For Windows users: this is Solitare
 */
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
    bool check_next_step_for_circle(const KlondikeStep&) const;

    SingleVectorGameState state_;
    Rules pile_rules;
    Rules foundation_rules;
    bool check_further;
};

std::ostream& operator<<(std::ostream&, const KlondikeGame&);
std::istream& operator>>(std::istream&, KlondikeGame&);

#endif
