#include <iostream>
#include <sstream>
#include <typeinfo>
#include <algorithm>

#include "ScorpionGame.hpp"
#include "Log.hpp"
#include "Trace.hpp"
#include "crc.hpp"

static const unsigned PILES = 8;
static const unsigned GAME_PILES = 7;
static const unsigned TURNED_GAME_PILES = 3;
static const unsigned DOWN_TURNED_CARDS = 2;
static const unsigned PILE_SIZE = 7;
static const unsigned STOCK_SIZE = 3;
static const unsigned STOCK_MOVE = 7;
static const unsigned STOCK_PILE = 7;
static const unsigned STATE_SIZE = Deck::DECK_SIZE + GAME_PILES; // 52 card + 7 index values for pile_start
static const CardCode ACE = 0;
static const CardCode KING = 12;

ScorpionGame::ScorpionGame() : state(STATE_SIZE, GAME_PILES), rules() {}

ScorpionGame::ScorpionGame(Deck& deck, bool shuffle) : state(STATE_SIZE, PILES), rules() {
    if (shuffle)
        deck.shuffle();
    int i = state.first_card_pos(), p = 0;
    deck.deal([&] (const Card& c) {
        Card card(c);
        if ((p < TURNED_GAME_PILES && (i - state.first_card_pos()) % PILE_SIZE < DOWN_TURNED_CARDS) || p == GAME_PILES)
            card.turnup(false);
        state[i++] = card.get();
        if ((i - state.first_card_pos()) % PILE_SIZE == 0) {
            state[p++] = i;
        }
    });
}

bool ScorpionGame::operator==(const Game& g) const {
    try {
        const ScorpionGame& sg = dynamic_cast<const ScorpionGame&>(g);
        return state == sg.state;
    } catch (const std::bad_cast& e) {
        return false;
    }
}

ScorpionGame::operator bool() const {
    return (state) ? true : false;
}

std::size_t ScorpionGame::hash() const {
    return state.hash();
}

void ScorpionGame::do_step(const ScorpionStep& s) {
    Trace(TraceComponent::GAME) << "DO " << s;
    if (s.is_stock_move()) {
        do_stock_move();
    } else {
        do_move_and_upturn(s);
    }
    Trace(TraceComponent::GAME) << *this;
}

void ScorpionGame::undo_step(const ScorpionStep& s) {
    Trace(TraceComponent::GAME) << "UNDO " << s;
    if (s.is_stock_move()) {
        undo_stock_move();
    } else {
        undo_move_and_upturn(s);
    }
    Trace(TraceComponent::GAME) << *this;
}

/**
 * @brief Valid steps:
 * @details
 * * part of a pile can be moved if the bottom of it is a rank below the top of the other pile
 * * king can be moved to an empty pile
 * * stock can be spread to the first 3 piles
 */
std::vector<ScorpionStep> ScorpionGame::valid_steps() const {
    std::vector<ScorpionStep> steps;
    if (!is_four_pile_all_ace()) {
        for (unsigned from = 0; from < GAME_PILES; ++from) {
            for (unsigned to = 0; to < GAME_PILES; ++to) {
                if (from == to || state.pile_empty(from) || Card(state[state.pile_top(to)]).rank() == ACE)
                    continue;
                unsigned prev = state.pile_bottom(from);
                for (unsigned i = state.pile_top(from); i >= state.pile_bottom(from); --i) {
                    // upturn downturned card if the previous step revealed it
                    if (!Card(state[i])) {
                        if (!steps.empty() && state[prev] == steps.back().card_code()) {
                            steps.back().turned_up(true);
                        }
                        prev = i;
                        continue;
                    }
                    // normal step
                    if (!state.pile_empty(to) && rules.is_before(state[i], state[state.pile_top(to)])) {
                        steps.emplace_back(state[i], from, to, i - state.pile_bottom(from), state.pile_size(to));
                        Trace(TraceComponent::GAME) << "from " << state(from) << ", to " << state(to) << " ? " << state.pile_size(to);
                    }
                    // king step
                    if (state.pile_empty(to) && Card(state[i]).rank() == KING) {
                        // don't move already king topped pile after stock move, makes no difference
                        if (state.pile_empty(STOCK_PILE) && state[i] == state[state.pile_bottom(from)])
                            continue;
                        steps.emplace_back(state[i], from, to, i - state.pile_bottom(from), state.pile_size(to));
                    }
                    prev = i;
                }
            }
        }
        // stock move
        if (!state.pile_empty(STOCK_PILE))
            steps.emplace_back(ScorpionStep::STOCK_MOVE);
    }
    return steps;
}

/// Game is won when all non-empty piles has ace on top and cards are in order
bool ScorpionGame::win() const {
    if (!state.pile_empty(STOCK_PILE))
        return false;
    for (unsigned p = 0; p < GAME_PILES; ++p) {
        if (state.pile_size(p) > 0 && state.pile_size(p) < 13)
            return false;
        if (state.pile_empty(p))
            continue;
        if (Card(state[state.pile_top(p)]).rank() != ACE)
            return false;
        unsigned card = state.pile_top(p);
        unsigned prev = card--;
        do {
            if (!rules.is_before(state[prev], state[card]))
                return false;
            prev = card;
        } while (--card != state.pile_bottom(p));
    }
    return true;
}

bool ScorpionGame::sanity() const {
    return !deadlock();
}

void ScorpionGame::do_stock_move() {
    for (unsigned i = 0; i < TURNED_GAME_PILES; ++i) {
        state.move_single_card_forward(STOCK_PILE, i, TURNED_GAME_PILES-i-1, state.pile_size(i));
        Card::turnup(state[state.pile_top(i)]);
    }
}

void ScorpionGame::undo_stock_move() {
    for (unsigned i = 0; i < TURNED_GAME_PILES; ++i) {
        auto pile = TURNED_GAME_PILES - i - 1;
        Card::turnup(state[state.pile_top(pile)], false);
        state.move_single_card_backward(pile, STOCK_PILE, state.pile_size(pile) - 1, i);
    }
}

void ScorpionGame::do_move_and_upturn(const ScorpionStep& s) {
    state.do_move_and_upturn(s);
}

void ScorpionGame::undo_move_and_upturn(const ScorpionStep& s) {
    state.undo_move_and_upturn(s);
}

bool ScorpionGame::is_four_pile_all_ace() const {
    unsigned n_pile = 0, n_ace = 0, prev_pile = state[0];
    for (unsigned p = 1, prev = state[0]; p < GAME_PILES; prev = state[p++]) {
        if (state[p] > prev) {
            n_pile++;
            n_ace += (Card(state[state.pile_top(p)]).rank() == ACE) ? 1 : 0;
        }
    }
    return (n_pile == 4 && n_ace == 4 && state.pile_empty(STOCK_PILE));
}

bool ScorpionGame::deadlock() const {
    unsigned p = 0;
    for (unsigned i = state.first_card_pos(); i < STATE_SIZE; ++i) {
        // only upturned cards
        if (!Card(state[i]))
            continue;
        // change piles at top
        if (i == state.pile_top(p)) {
            ++p;
            continue;
        }
        // skip valid sequences or kings
        if (rules.is_before(state[i+1], state[i]) || Card(state[i+1]).rank() == KING)
            continue;

        unsigned next_pos = state.find_card(rules.next(state[i+1])[0]);
        if (next_pos == Card::CARD_SEPARATOR)
            continue;
        while (next_pos != Card::CARD_SEPARATOR && state[i] != state[next_pos]) {
            if (!Card(state[next_pos]))
                break;
            bool at_pile_top = false;
            for (unsigned p = 0; p < GAME_PILES; ++p)
                at_pile_top |= (next_pos == state.pile_top(p));
            if (at_pile_top)
                break;
            if (Card(state[next_pos+1]).rank() == KING)
                break;

            next_pos = state.find_card(rules.next(state[next_pos+1])[0]);
        }

        if (next_pos != Card::CARD_SEPARATOR && state[i] == state[next_pos]) {
            Log(Log::INFO) << "DEADLOCK " << Card(state[i]) << std::endl;
            return true;
        }
    }
    return false;
}

unsigned ScorpionGame::locked_down_turned() const {
    unsigned result = 0;
    GameState down_turned_cards;
    for (unsigned p = 0; p < TURNED_GAME_PILES; ++p) {
        for (unsigned i = 0; i < DOWN_TURNED_CARDS; ++i) {
            CardCode cc = state[state.pile_bottom(p) + i];
            // if (!Card::upturned(cc))
            if (!Card(cc))
                down_turned_cards.push_back(cc);
        }
    }
    for (unsigned p = 0; p < TURNED_GAME_PILES; ++p) {
        for (unsigned i = 0; i < TURNED_GAME_PILES*DOWN_TURNED_CARDS; ++i) {
            if (rules.is_before(state[state.pile_bottom(p) + DOWN_TURNED_CARDS], down_turned_cards[i]))
                result++;
        }
    }
    return result;
}

std::ostream& ScorpionGame::print(std::ostream& os) const {
    unsigned i = state.first_card_pos();
    for (unsigned p = 0; p < GAME_PILES; ++p) {
        os << p+1 << ":";
        for (; i < state[p]; ++i) {
            os << Card(state[i]);
            if (i < state[p] - 1)
                os << " ";
        }
        os << '\n';
    }
    os << "S:";
    for (; i<STATE_SIZE; ++i) {
        os << Card(state[i]);
        if (i < STATE_SIZE - 1)
            os << " ";
    }
    return os;
}

std::istream& ScorpionGame::read(std::istream& is) {
    state.reset(STATE_SIZE, PILES);
    unsigned i = state.first_card_pos();
    for (unsigned p = 0; p <= GAME_PILES; ++p) {
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
        if (p < GAME_PILES)
            state[p] = i;
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const ScorpionGame& g) {
    return g.print(os);
}

std::istream& operator>>(std::istream& is, ScorpionGame& g) {
    return g.read(is);
}
