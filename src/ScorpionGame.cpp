#include <iostream>

#include "ScorpionGame.hpp"

static const int PILES = 7;
static const int TURNED_PILES = 3;
static const int STOCK_SIZE = 3;
static const unsigned char STOCK_MOVE = 7;

bool ScorpionStep::is_stock_move() {
    return (from == STOCK_MOVE && to == STOCK_MOVE);
}

ScorpionGame::ScorpionGame() : piles(PILES) {
    Deck deck;
    deck.shuffle();
    deal(deck);
}

ScorpionGame::ScorpionGame(Deck& deck) : piles(PILES) {
    deal(deck);
}

void ScorpionGame::deal(Deck& d) {
    for (int i = 0; i < PILES; ++i) {
        if (i < TURNED_PILES) {
            piles[i] = d.deal(2, 5);
        } else {
            piles[i] = d.deal(7, true);
        }
    }
    stock = d.deal(3, false);
}

bool ScorpionGame::operator==(const Game& g) {
    try {
        const ScorpionGame& sg = dynamic_cast<const ScorpionGame&>(g);
        bool e = true;
        for (int i = 0; i < PILES; ++i) {
            e &= std::equal(piles[i].begin(), piles[i].end(), sg.piles[i].begin());
        }
        return e && std::equal(stock.begin(), stock.end(), sg.stock.begin());
    } catch (const std::bad_cast& e) {
        return false;
    }
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

std::list<ScorpionStep> ScorpionGame::valid_steps() {
    std::list<ScorpionStep> steps;

    return steps;
}

bool ScorpionGame::end() {
    return true;
}

void ScorpionGame::do_stock_move() {
    for (int i = 0; i < STOCK_SIZE; ++i) {
        stock.front().turnup(true);
        piles[i].splice(piles[i].begin(), stock, stock.begin());
    }
}

void ScorpionGame::undo_stock_move() {
    for (int i = 0; i < STOCK_SIZE; ++i) {
        piles[i].front().turnup(false);
        stock.splice(stock.end(), piles[i], piles[i].begin());
    }
}

void ScorpionGame::do_move_and_upturn(ScorpionStep& s) {
    for (auto c = piles[s.from].begin(); c != piles[s.from].end(); ++c) {
        if (*c == s.card) {
            piles[s.to].splice(piles[s.to].begin(), piles[s.from], piles[s.from].begin(), ++c);
            if (piles[s.from].size() > 0 && !piles[s.from].front().upturned()) {
                piles[s.from].front().turnup(true);
            }
            return;
        }
    }
}

void ScorpionGame::undo_move_and_upturn(ScorpionStep& s) {
    for (auto c = piles[s.to].begin(); c != piles[s.to].end(); ++c) {
        if (*c == s.card) {
            if (s.turnedup) {
                piles[s.from].front().turnup(false);
            }
            piles[s.from].splice(piles[s.from].begin(), piles[s.to], piles[s.to].begin(), ++c);
            return;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const ScorpionGame& g) {
    for (int i = 0; i < PILES; ++i) {
        os << g.piles[i] << std::endl;
    }
    os << g.stock << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, ScorpionGame& g) {
    for (int i = 0; i < PILES; ++i) {
        g.piles[i].clear();
        is >> g.piles[i];
    }
    g.stock.clear();
    is >> g.stock;
    return is;
}
