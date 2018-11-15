#include <iostream>
#include <sstream>
#include <typeinfo>
#include <algorithm>

#include "ScorpionGame.hpp"
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
    if (s.is_stock_move()) {
        do_stock_move();
    } else {
        do_move_and_upturn(s);
    }
}

void ScorpionGame::undo_step(const ScorpionStep& s) {
    if (s.is_stock_move()) {
        undo_stock_move();
    } else {
        undo_move_and_upturn(s);
    }
}

std::vector<ScorpionStep> ScorpionGame::valid_steps() const {
    std::vector<ScorpionStep> steps;
    // std::cout << "valid:" << steps.size() << std::endl;
    if (!is_four_pile_all_ace()) {
        for (unsigned from = 0; from < GAME_PILES; ++from) {
            for (unsigned to = 0; to < GAME_PILES; ++to) {
                // std::cout << from+1 << "->" << to+1 << " e" << state.pile_empty(from) << " c" << Card(state[state.pile_top(to)]) << std::endl;
                if (from == to || state.pile_empty(from) || Card(state[state.pile_top(to)]).rank() == ACE)
                    continue;
                // std::cout << from+1 << "->" << to+1 << ": ";
                unsigned prev = state.pile_bottom(from);
                for (unsigned i = state.pile_top(from); i >= state.pile_bottom(from); --i) {
                    // std::cout <<  Card(state[i]) << " - " << Card(state[state.pile_top(to)]) << " ";
                    // std::cout << state.pile_top(from) << " " << state.pile_bottom(from) << " " << i << std::endl;
                    if (!Card::upturned(state[i])) {
                        if (!steps.empty() && state[prev] == steps.back().card_code()) {
                            steps.back().turned_up(true);
                        }
                        prev = i;
                        continue;
                    }
                    // normal step
                    if (!state.pile_empty(to) && rules.is_before(state[i], state[state.pile_top(to)])) {
                        steps.emplace_back(state[i], from, to, i - state.pile_bottom(from), state.pile_size(to));
                        // std::cout << "add step:" << steps.back() << " s:" << steps.size() << std::endl;
                    }
                    // king step
                    if (state.pile_empty(to) && Card(state[i]).rank() == KING) {
                        // don't move already king topped pile after stock move, makes no difference
                        if (state.pile_empty(STOCK_PILE) && state[i] == state[state.pile_bottom(from)])
                            continue;
                        steps.emplace_back(state[i], from, to, i - state.pile_bottom(from), state.pile_size(to));
                        // std::cout << "add step:" << steps.back() << " s:" << steps.size() << std::endl;
                    }
                    prev = i;
                }
                // std::cout << std::endl;
            }
        }
        // stock move
        if (!state.pile_empty(STOCK_PILE))
            steps.emplace_back(ScorpionStep::STOCK_MOVE);
    }
    // std::cout << "valid:" << steps.size() << std::endl;
    return steps;
}

bool ScorpionGame::win() const {
    if (!state.pile_empty(STOCK_PILE))
        return false;
    for (unsigned p = 0; p < GAME_PILES; ++p) {
        // std::cout << " p" << state[p] << " z" << state.pile_size(p) << " e" << state.pile_empty(p) << " c" << Card(state[state.pile_top(p)]) << std::endl;
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
    unsigned end0 = state.pile_size(0);
    unsigned end1 = state.pile_size(1);
    unsigned end2 = state.pile_size(2);
    // std::cout << "[" << end0 << ", " << end1 << ", " << end2 << "]" << std::endl;
    state.move_cards_forward(STOCK_PILE, 2, 0, end2);
    state.move_cards_forward(2, 1, end2+1, end1);
    state.move_cards_forward(1, 0, end1+1, end0);
    for (unsigned i = 0; i < TURNED_GAME_PILES; ++i)
        Card::turnup(state[state.pile_top(i)]);
}

void ScorpionGame::undo_stock_move() {
    for (unsigned i = 0; i < TURNED_GAME_PILES; ++i)
        Card::turnup(state[state.pile_top(i)], false);
    unsigned end0 = state.pile_size(0);
    unsigned end1 = state.pile_size(1);
    unsigned end2 = state.pile_size(2);
    // std::cout << "[" << end0 << ", " << end1 << ", " << end2 << "]" << std::endl;
    state.move_cards_backward(0, 1, end0-1, end1);
    state.move_cards_backward(1, 2, end1-1, end2);
    state.move_cards_backward(2, STOCK_PILE, end2-1, 0);
}

void ScorpionGame::do_move_and_upturn(const ScorpionStep& s) {
    if (s.pile_from() < s.pile_to()) {
        state.move_cards_backward(s.pile_from(), s.pile_to(), s.card_pos(), s.new_pos());
    } else {
        state.move_cards_forward(s.pile_from(), s.pile_to(), s.card_pos(), s.new_pos());
    }
    if (s.turned_up()) {
        Card::turnup(state[state.pile_bottom(s.pile_from()) + s.card_pos() - 1]);
    }
}

void ScorpionGame::undo_move_and_upturn(const ScorpionStep& s) {
    if (s.pile_from() < s.pile_to()) {
        state.move_cards_forward(s.pile_to(), s.pile_from(), s.new_pos(), s.card_pos());
    } else {
        state.move_cards_backward(s.pile_to(), s.pile_from(), s.new_pos(), s.card_pos());
    }
    if (s.turned_up()) {
        Card::turnup(state[state.pile_bottom(s.pile_from()) + s.card_pos() - 1], false);
    }
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
    SingleVectorGameState temp_state(state);
    unsigned p = 0;
    for (unsigned i = state.first_card_pos(); i < STATE_SIZE; ++i) {
        // std::cout << "CHECK:" << Card(temp_state[i]) << std::endl;
        if (!Card::upturned(temp_state[i]))
            continue;
        if (i == state.pile_top(p)) {
            ++p;
            continue;
        }
        if (rules.is_before(temp_state[i+1], temp_state[i]) || Card(temp_state[i+1]).rank() == KING)
            continue;
        // std::cout << "  FLOW:" << Card(temp_state[i]) << Card(temp_state[i+1]);
        unsigned next_pos = state.find_card(rules.next(temp_state[i+1])[0]);
        while (temp_state[i] != temp_state[next_pos]) {
            // std::cout << Card(temp_state[next_pos]);
            if (!Card::upturned(temp_state[next_pos]))
                break;
            bool at_pile_top = false;
            for (unsigned p = 0; p < GAME_PILES; ++p)
                at_pile_top |= (next_pos == state.pile_top(p));
            if (at_pile_top)
                break;
            if (Card(temp_state[next_pos+1]).rank() == KING)
                break;
            // std::cout << Card(temp_state[next_pos+1]);
            next_pos = state.find_card(rules.next(temp_state[next_pos+1])[0]);
        }
        // std::cout << std::endl;
        if (temp_state[i] == temp_state[next_pos]) {
            // std::cout << "DEADLOCK " << Card(state[i]) << std::endl;
            return true;
        }
    }
    // std::cout << "END_STATE:" << temp_state << std::endl;
    return false;
}

unsigned ScorpionGame::locked_down_turned() const {
    unsigned result = 0;
    GameState down_turned_cards;
    for (unsigned p = 0; p < TURNED_GAME_PILES; ++p) {
        for (unsigned i = 0; i < DOWN_TURNED_CARDS; ++i) {
            CardCode cc = state[state.pile_bottom(p) + i];
            if (!Card::upturned(cc))
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

std::ostream& operator<<(std::ostream& os, const ScorpionGame& g) {
    unsigned i = g.state.first_card_pos();
    for (unsigned p = 0; p < GAME_PILES; ++p) {
        os << p+1 << ":";
        for (; i < g.state[p]; ++i) {
            os << Card(g.state[i]);
            if (i < g.state[p] - 1)
                os << " ";
        }
        os << '\n';
    }
    os << "S:";
    for (; i<STATE_SIZE; ++i) {
        os << Card(g.state[i]);
        if (i < STATE_SIZE - 1)
            os << " ";
    }
    return os;
}

std::istream& operator>>(std::istream& is, ScorpionGame& g) {
    g.state.reset(STATE_SIZE, PILES);
    unsigned i = g.state.first_card_pos();
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
            g.state[i++] = c.get();
        }
        if (p < GAME_PILES)
            g.state[p] = i;
    }
    return is;
}
