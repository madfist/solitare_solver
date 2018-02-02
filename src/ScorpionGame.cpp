#include <iostream>
#include <sstream>
#include <typeinfo>
#include <algorithm>

#include "ScorpionGame.hpp"
#include "crc32.h"

static const unsigned PILES = 7;
static const unsigned TURNED_PILES = 3;
static const unsigned DOWN_TURNED_CARDS = 2;
static const unsigned PILE_SIZE = 7;
static const unsigned STOCK_SIZE = 3;
static const unsigned STOCK_MOVE = 7;
static const unsigned STOCK_PILE = 7;
static const unsigned STATE_SIZE = 59;
static const CardCode ACE = 0;
static const CardCode KING = 12;

ScorpionGame::ScorpionGame() : state(), rules() {}

ScorpionGame::ScorpionGame(const GameState& gs) : state(gs), rules() {}

ScorpionGame::ScorpionGame(Deck& deck, bool s) : state(STATE_SIZE), rules() {
    if (s)
        deck.shuffle();
    int i = PILES, p = 0;
    deck.deal([&] (const Card& c) {
        Card card(c);
        if ((p < TURNED_PILES && (i-PILES)%PILE_SIZE < DOWN_TURNED_CARDS) || p == PILES)
            card.turnup(false);
        state[i++] = card.get();
        if ((i-PILES)%PILE_SIZE == 0) {
            state[p++] = i;
        }
    });
}

bool ScorpionGame::operator==(const Game& g) const {
    try {
        const ScorpionGame& sg = dynamic_cast<const ScorpionGame&>(g);
        return std::equal(state.begin(), state.end(), sg.state.begin());
    } catch (const std::bad_cast& e) {
        return false;
    }
}

ScorpionGame::operator bool() const {
    return !state.empty();
}

std::size_t ScorpionGame::hash() const {
    if (sizeof(std::size_t) == 8)
        return crc64c(0, state.data(), STATE_SIZE);
    else
        return crc32c(0, state.data(), STATE_SIZE);

}

void ScorpionGame::do_step(ScorpionStep& s) {
    if (s.is_stock_move()) {
        do_stock_move();
    } else {
        do_move_and_upturn(s);
    }
}

void ScorpionGame::undo_step(ScorpionStep& s) {
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
        for (unsigned from = 0; from < PILES; ++from) {
            for (unsigned to = 0; to < PILES; ++to) {
                if (from == to || pile_empty(from) || Card(state[pile_top(to)]).rank() == ACE)
                    continue;
                // std::cout << from+1 << "->" << to+1 << ": ";
                unsigned prev = pile_bottom(from);
                for (unsigned i = pile_top(from); i >= pile_bottom(from); --i) {
                    // std::cout <<  Card(state[i]) << " - " << Card(state[pile_top(to)]) << " ";
                    if (!Card::upturned(state[i])) {
                        if (!steps.empty() && state[prev] == steps.back().card_code()) {
                            steps.back().turned_up(true);
                        }
                        prev = i;
                        continue;
                    }
                    // normal step
                    if (!pile_empty(to) && rules.is_before(state[i], state[pile_top(to)])) {
                        steps.push_back(ScorpionStep(state[i], from, to, i - pile_bottom(from), pile_size(to)));
                        // std::cout << "add step:" << steps.back() << " s:" << steps.size() << std::endl;
                    }
                    // king step
                    if (pile_empty(to) && Card(state[i]).rank() == KING) {
                        // don't move already king topped pile after stock move, makes no difference
                        if (pile_empty(STOCK_PILE) && state[i] == state[pile_bottom(from)])
                            continue;
                        steps.push_back(ScorpionStep(state[i], from, to, i - pile_bottom(from), pile_size(to)));
                        // std::cout << "add step:" << steps.back() << " s:" << steps.size() << std::endl;
                    }
                    prev = i;
                }
                // std::cout << std::endl;
            }
        }
        // stock move
        if (!pile_empty(STOCK_PILE))
            steps.push_back(ScorpionStep(ScorpionStep::STOCK_MOVE));
    }
    // std::cout << "valid:" << steps.size() << std::endl;
    return steps;
}

bool ScorpionGame::win() const {
    if (!pile_empty(STOCK_PILE))
        return false;
    for (unsigned p = 0; p < PILES; ++p) {
        // std::cout << " p" << state[p] << " z" << pile_size(p) << " e" << pile_empty(p) << " c" << Card(state[pile_top(p)]) << std::endl;
        if (pile_size(p) > 0 && pile_size(p) < 13)
            return false;
        if (pile_empty(p))
            continue;
        if (Card(state[pile_top(p)]).rank() != ACE)
            return false;
        unsigned card = pile_top(p);
        unsigned prev = card--;
        do {
            if (!rules.is_before(state[prev], state[card]))
                return false;
            prev = card;
        } while (--card != pile_bottom(p));
    }
    return true;
}

bool ScorpionGame::sanity() const {
    return !deadlock();
}

void ScorpionGame::do_stock_move() {
    unsigned end0 = pile_size(0);
    unsigned end1 = pile_size(1);
    unsigned end2 = pile_size(2);
    // std::cout << "[" << end0 << ", " << end1 << ", " << end2 << "]" << std::endl;
    move_cards_forward(STOCK_PILE, 2, 0, end2);
    move_cards_forward(2, 1, end2+1, end1);
    move_cards_forward(1, 0, end1+1, end0);
    for (unsigned i = 0; i < TURNED_PILES; ++i)
        Card::turnup(state[pile_top(i)]);
}

void ScorpionGame::undo_stock_move() {
    for (unsigned i = 0; i < TURNED_PILES; ++i)
        Card::turnup(state[pile_top(i)], false);
    unsigned end0 = pile_size(0);
    unsigned end1 = pile_size(1);
    unsigned end2 = pile_size(2);
    // std::cout << "[" << end0 << ", " << end1 << ", " << end2 << "]" << std::endl;
    move_cards_backward(0, 1, end0-1, end1);
    move_cards_backward(1, 2, end1-1, end2);
    move_cards_backward(2, STOCK_PILE, end2-1, 0);
}

void ScorpionGame::do_move_and_upturn(ScorpionStep& s) {
    if (s.pile_from() < s.pile_to()) {
        move_cards_backward(s.pile_from(), s.pile_to(), s.card_pos(), s.new_pos());
    } else {
        move_cards_forward(s.pile_from(), s.pile_to(), s.card_pos(), s.new_pos());
    }
    if (s.turned_up()) {
        Card::turnup(state[pile_bottom(s.pile_from()) + s.card_pos() - 1]);
    }
}

void ScorpionGame::undo_move_and_upturn(ScorpionStep& s) {
    unsigned first, middle, last, diff;
    if (s.pile_from() < s.pile_to()) {
        move_cards_forward(s.pile_to(), s.pile_from(), s.new_pos(), s.card_pos());
    } else {
        move_cards_backward(s.pile_to(), s.pile_from(), s.new_pos(), s.card_pos());
    }
    if (s.turned_up()) {
        Card::turnup(state[pile_bottom(s.pile_from()) + s.card_pos() - 1], false);
    }
}

bool ScorpionGame::is_four_pile_all_ace() const {
    unsigned n_pile = 0, n_ace = 0, prev_pile = state[0];
    for (unsigned p = 1, prev = state[0]; p < PILES; prev = state[p++]) {
        if (state[p] > prev) {
            n_pile++;
            n_ace += (Card(state[pile_top(p)]).rank() == ACE) ? 1 : 0;
        }
    }
}

bool ScorpionGame::pile_empty(unsigned p) const {
    if (p == 0)
        return (state[p] == PILES);
    if (p == STOCK_PILE)
        return (state[p-1] == STATE_SIZE);
    return state[p] == state[p-1];
}

unsigned ScorpionGame::pile_size(unsigned p) const {
    return pile_top(p) - pile_bottom(p) + 1;
}

unsigned ScorpionGame::pile_bottom(unsigned n) const {
    return (n == 0) ? PILES : state[n-1];
}

unsigned ScorpionGame::pile_top(unsigned n) const {
    return (n == STOCK_PILE) ? STATE_SIZE-1 : state[n]-1;
}

void ScorpionGame::move_cards_backward(unsigned from, unsigned to, unsigned card_pos, unsigned new_pos) {
    unsigned first, middle, last, diff;
    first = pile_bottom(from) + card_pos;
    middle = pile_top(from) + 1;
    last = pile_bottom(to) + new_pos;
    diff = middle - first;
    // std::cout << "mb f" << first << " m" << middle << " l" << last << " d" << diff << std::endl;

    auto st = state.begin();
    std::rotate(st + first, st + middle, st + last);

    for (unsigned p = from; p < to; ++p)
        state[p] -= diff;
}

void ScorpionGame::move_cards_forward(unsigned from, unsigned to, unsigned card_pos, unsigned new_pos) {
    unsigned first, middle, last, diff;
    first = pile_bottom(to) + new_pos;
    middle = pile_bottom(from) + card_pos;
    last = pile_top(from) + 1;
    diff = last - middle;
    // std::cout << "mf f" << first << " m" << middle << " l" << last << " d" << diff << std::endl;

    auto st = state.begin();
    std::rotate(st + first, st + middle, st + last);

    for (unsigned p = to; p < from; ++p)
        state[p] += diff;
}

bool ScorpionGame::deadlock() const {
    GameState temp_state(state);
    unsigned p = 0;
    for (unsigned i = PILES; i < STATE_SIZE; ++i) {
        // std::cout << "CHECK:" << Card(temp_state[i]) << std::endl;
        if (!Card::upturned(temp_state[i]))
            continue;
        if (i == pile_top(p)) {
            ++p;
            continue;
        }
        if (rules.is_before(temp_state[i+1], temp_state[i]) || Card(temp_state[i+1]).rank() == KING)
            continue;
        // std::cout << "  FLOW:" << Card(temp_state[i]) << Card(temp_state[i+1]);
        unsigned next_pos = find_card(rules.next(temp_state[i+1])[0]);
        while (temp_state[i] != temp_state[next_pos]) {
            // std::cout << Card(temp_state[next_pos]);
            if (!Card::upturned(temp_state[next_pos]))
                break;
            bool at_pile_top = false;
            for (unsigned p = 0; p < PILES; ++p)
                at_pile_top |= (next_pos == pile_top(p));
            if (at_pile_top)
                break;
            if (Card(temp_state[next_pos+1]).rank() == KING)
                break;
            // std::cout << Card(temp_state[next_pos+1]);
            next_pos = find_card(rules.next(temp_state[next_pos+1])[0]);
        }
        // std::cout << std::endl;
        if (temp_state[i] == temp_state[next_pos]) {
            std::cout << "DEADLOCK " << Card(state[i]) << std::endl;
            return true;
        }
    }
    // std::cout << "END_STATE:" << temp_state << std::endl;
    return false;
}

unsigned ScorpionGame::find_card(const CardCode& cc) const {
    for (unsigned i = 0; i < STATE_SIZE; ++i) {
        if (state[i] == cc)
            return i;
    }
}

unsigned ScorpionGame::locked_down_turned() const {
    unsigned result = 0;
    GameState down_turned_cards;
    for (unsigned p = 0; p < TURNED_PILES; ++p) {
        for (unsigned i = 0; i < DOWN_TURNED_CARDS; ++i) {
            CardCode cc = state[pile_bottom(p) + i];
            if (!Card::upturned(cc))
                down_turned_cards.push_back(cc);
        }
    }
    for (unsigned p = 0; p < TURNED_PILES; ++p) {
        for (unsigned i = 0; i < TURNED_PILES*DOWN_TURNED_CARDS; ++i) {
            if (rules.is_before(state[pile_bottom(p) + DOWN_TURNED_CARDS], down_turned_cards[i]))
                result++;
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const ScorpionGame& g) {
    unsigned i = PILES;
    for (unsigned p = 0; p < PILES; ++p) {
        std::cout << p+1 << ":";
        for (; i < g.state[p]; ++i) {
            os << Card(g.state[i]);
            if (i < g.state[p] - 1)
                os << " ";
        }
        os << std::endl;
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
    g.state.clear();
    g.state.resize(STATE_SIZE);
    unsigned i = PILES;
    for (unsigned p = 0; p <= PILES; ++p) {
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
        if (p < PILES)
            g.state[p] = i;
    }
    return is;
}
