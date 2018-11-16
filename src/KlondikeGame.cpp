#include <iostream>
#include <sstream>
#include <iterator>

#include "KlondikeGame.hpp"

static const unsigned STATE_SIZE = Deck::DECK_SIZE + 11; // 52 cards + 11 index values for piles_start
static const unsigned GAME_PILES = 7;
static const unsigned FOUNDATION_PILES = 4;
static const unsigned STOCK_PILE = 11;
static const unsigned ALL_PILES = 12;

static const CardCode KING = 12;
static const CardCode ACE = 0;

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

void KlondikeGame::do_step(const KlondikeStep& s) {
    if (s.is_stock_move()) {
        state.move_single_card_forward(s.pile_from(), s.pile_to(), s.card_pos(), s.new_pos());
    } else {
        state.do_move_and_upturn(s);
    }
}

void KlondikeGame::undo_step(const KlondikeStep& s) {
    if (s.is_stock_move()) {
        state.move_single_card_backward(s.pile_to(), s.pile_from(), s.new_pos(), s.card_pos());
    } else {
        state.undo_move_and_upturn(s);
    }
}

std::vector<KlondikeStep> KlondikeGame::valid_steps() const {
    std::vector<KlondikeStep> steps;

    for (pile from = 0; from < ALL_PILES; ++from) {
        for (pile to = 0; to < GAME_PILES + FOUNDATION_PILES; ++to) {
            if (from == to || state(from).empty() || (from >= GAME_PILES && from != STOCK_PILE && to >= GAME_PILES))
                continue;
            state(from).top_to_bottom([&](unsigned i, const CardCode& cc) {
                unsigned weight = 3;
                bool step_found = false;
                if (!Card::upturned(cc) || (i < state.pile_top(from) && to >= GAME_PILES))
                    return;
                // king/ace step
                if (state(to).empty()) {
                    if (Card(cc).rank() == KING && to < GAME_PILES) {
                        // don't move kings between empty piles
                        if (cc == state(from).bottom())
                            return;
                        weight = 2;
                        step_found = true;
                    }
                    else if (Card(cc).rank() == ACE && to >= GAME_PILES) {
                        // don't move aces to more than one empty pile
                        if (to - 1 >= GAME_PILES && state(to - 1).empty())
                            return;
                        weight = 3;
                        step_found = true;
                    }
                }
                // normal step
                else {
                    step_found = (to < GAME_PILES && pile_rules.is_before(state[i], state(to).top())) || (to >= GAME_PILES && foundation_rules.is_before(state(to).top(), state(from).top()));
                    if (i > state.pile_bottom(from) && Card(state[i-1]).rank() == Card(state(to).top()).rank() || from >= GAME_PILES) {
                        weight = 0;
                    }
                    else if (to >= GAME_PILES){
                        weight = 2;
                    }
                }
                if (step_found) {
                    steps.emplace_back(cc, from, to, i - state.pile_bottom(from), state(to).size(), (from == STOCK_PILE) ? 2 : weight, (from == STOCK_PILE));
                    if (i > state.pile_bottom(from) && !Card::upturned(state[i-1]) && state[i] == steps.back().card_code())
                        steps.back().turned_up(true);
                }
            });
        }
    }

    std::sort(steps.begin(), steps.end());
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

std::ostream& KlondikeGame::print(std::ostream& os) const {
    unsigned i = state.first_card_pos();
    for (unsigned p = 0; p < GAME_PILES; ++p) {
        std::cout << " " << p+1 << ":";
        for (; i < state[p]; ++i) {
            os << Card(state[i]);
            if (i < state[p] - 1)
                os << " ";
        }
        os << '\n';
    }
    for (unsigned p = GAME_PILES; p < GAME_PILES + FOUNDATION_PILES; ++p) {
        std::cout << "F" << p+1-GAME_PILES << ":";
        for (; i < state[p]; ++i) {
            os << Card(state[i]);
            if (i < state[p] - 1)
                os << " ";
        }
        os << '\n';
    }
    os << " S:";
    for (; i<STATE_SIZE; ++i) {
        os << Card(state[i]);
        if (i < STATE_SIZE - 1)
            os << " ";
    }
    return os;
}

std::istream& KlondikeGame::read(std::istream& is) {
    state.reset(STATE_SIZE, ALL_PILES);
    unsigned i = state.first_card_pos();
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
            state[i++] = c.get();
        }
        if (p < GAME_PILES + FOUNDATION_PILES)
            state[p] = i;
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const KlondikeGame& g) {
    return g.print(os);
}

std::istream& operator>>(std::istream& is, KlondikeGame& g) {
    return g.read(is);
}