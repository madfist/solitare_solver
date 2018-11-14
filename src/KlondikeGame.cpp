#include <iostream>
#include <sstream>
#include "KlondikeGame.hpp"

static const unsigned STATE_SIZE = Deck::DECK_SIZE + 11; // 52 cards + 11 index values for piles_start
static const unsigned GAME_PILES = 7;
static const unsigned FOUNDATION_PILES = 4;
static const unsigned STOCK_PILE = 11;
static const unsigned ALL_PILES = 12;

static const CardCode KING = 12;

KlondikeGame::KlondikeGame()
        : state()
        , pile_rules(Rules::ALTERNATE, Rules::ACE_KING_DISABLED)
        , foundation_rules() {}

KlondikeGame::KlondikeGame(Deck& deck, bool shuffle /*false*/)
        : state(STATE_SIZE, ALL_PILES)
        , pile_rules(Rules::ALTERNATE, Rules::ACE_KING_DISABLED)
        , foundation_rules() {
    if (shuffle)
        deck.shuffle();
    unsigned i = state.first_card_pos(), pile = 0, pile_size = 1, dealt = 0;
    state[STOCK_PILE] = STATE_SIZE;
    deck.deal([&] (const Card& c) {
        Card card(c);
        if (pile < GAME_PILES) {
            // std::cout << "i" << i << " p" << pile << " s" << pile_size << " d" << dealt << " c" << card << std::endl;
            if (i < state.first_card_pos() + dealt + pile_size - 1) {
                card.turnup(false);
            }
            state[i++] = card.get();
            if (i == state.first_card_pos() + dealt + pile_size) {
                state[pile++] = i;
                dealt += pile_size++;
            }
        } else {
            if (pile < state.first_card_pos()) {
                state[pile++] = dealt + state.first_card_pos();
            }
            // STOCK
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
    // check for folded cards in the piles. If there isn't any the game is practically done.
    for (unsigned p = 0; p < GAME_PILES; ++p) {
        if (state.pile_empty(p)) {
            continue;
        }
        for (unsigned i = state.pile_bottom(p); i < state.pile_top(p); ++i) {
            if (!Card::upturned(state[i])) {
                return false;
            }
        }
    }
    return true;
}

bool KlondikeGame::sanity() const {
    // no method to decide from initial state
    return true;
}

std::ostream& operator<<(std::ostream& os, const KlondikeGame& g) {
    unsigned i = g.state.first_card_pos();
    for (unsigned p = 0; p < GAME_PILES; ++p) {
        std::cout << " " << p+1 << ":";
        for (; i < g.state[p]; ++i) {
            os << Card(g.state[i]);
            if (i < g.state[p] - 1)
                os << " ";
        }
        os << '\n';
    }
    for (unsigned p = GAME_PILES; p < GAME_PILES + FOUNDATION_PILES; ++p) {
        std::cout << "F" << p+1-GAME_PILES << ":";
        for (; i < g.state[p]; ++i) {
            os << Card(g.state[i]);
            if (i < g.state[p] - 1)
                os << " ";
        }
        os << '\n';
    }
    os << " S:";
    for (; i<STATE_SIZE; ++i) {
        os << Card(g.state[i]);
        if (i < STATE_SIZE - 1)
            os << " ";
    }
    return os;
}

std::istream& operator>>(std::istream& is, KlondikeGame& g) {
    g.state.reset(STATE_SIZE, ALL_PILES);
    unsigned i = g.state.first_card_pos();
    for (unsigned p = 0; p <= GAME_PILES + FOUNDATION_PILES; ++p) {
        std::string line;
        std::getline(is, line);
        if (line.find(':') != std::string::npos)
            line = line.substr(2, line.length()-2);
        std::stringstream ss(line);

        if (ss.peek() == '\r')
            ss.get();
        if (ss.peek() == EOF)
            ss.get();
        while(!ss.eof() && ss.peek() != '\r') {
            Card c;
            ss >> c;
            g.state[i++] = c.get();
        }
        if (p < GAME_PILES + FOUNDATION_PILES)
            g.state[p] = i;
    }
    return is;
}