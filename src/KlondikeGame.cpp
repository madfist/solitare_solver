#include "KlondikeGame.hpp"

static const unsigned STATE_SIZE = 64; // 52 cards + 12 index values for piles_start
static const unsigned PILES = 7;
static const unsigned FOUNDATION_PILES = 4;
static const unsigned STOCK_PILE = 11;
static const unsigned CARDS = 12;

KlondikeGame::KlondikeGame()
        : state()
        , pile_rules(Rules::ALTERNATE, Rules::ACE_KING_DISABLED)
        , foundation_rules() {}

KlondikeGame::KlondikeGame(Deck& deck, bool shuffle)
        : state(STATE_SIZE, CARDS)
        , pile_rules(Rules::ALTERNATE, Rules::ACE_KING_DISABLED)
        , foundation_rules() {
    if (shuffle)
        deck.shuffle();
    unsigned i = CARDS, pile = 0, pile_size = 1, dealt = 0;
    state[STOCK_PILE] = STATE_SIZE;
    deck.deal([&] (const Card& c) {
        Card card(c);
        if (pile < PILES) {
            if (i < dealt + pile_size - 1) {
                card.turnup(false);
            }
            state[i++] = card.get();
            if (i == dealt + pile_size) {
                state[pile++] = i;
                dealt += pile_size++;
            }
        } else if (pile < PILES + FOUNDATION_PILES) {
            state[pile++] = i;
        } else { // STOCK
            card.turnup(false);
            state[i++] = card.get();
        }
    });
}

bool KlondikeGame::operator==(const Game& g) const {
    try {
        const KlondikeGame& kg = dynamic_cast<const KlondikeGame&>(g);
        return state == kg.state;
    } catch (const std::bad_cast& e) {
        return false;
    }
}

KlondikeGame::operator bool() const {
    return (state) ? true : false;
}

std::size_t KlondikeGame::hash() const {
    return state.hash();
}

void KlondikeGame::do_step(KlondikeStep&) {}
void KlondikeGame::undo_step(KlondikeStep&) {}

std::vector<KlondikeStep> KlondikeGame::valid_steps() const {
    std::vector<KlondikeStep> steps;
    return steps;
}

bool KlondikeGame::win() const {
    return false;
}

bool KlondikeGame::sanity() const {
    return true;
}