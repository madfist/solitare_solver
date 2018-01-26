#include <iostream>
#include <sstream>
#include <typeinfo>

#include "ScorpionGame.hpp"

static const int PILES = 7;
static const int TURNED_PILES = 3;
static const int STOCK_SIZE = 3;
static const unsigned STOCK_MOVE = 7;
static const CardCode ACE = 0;
static const CardCode KING = 12;

ScorpionStep::ScorpionStep(unsigned f, unsigned t, const Card& c, bool u)
    : from(f), to(t), card(c), turnedup(u) {}

bool ScorpionStep::is_stock_move() {
    return (from == STOCK_MOVE && to == STOCK_MOVE);
}

std::ostream& operator<<(std::ostream& os, const ScorpionStep& s) {
    os << "f:" << s.from << "_t:" << s.to << "_c:" << s.card;
    return os;
}

ScorpionGame::ScorpionGame() : piles(PILES), rules() {}

ScorpionGame::ScorpionGame(Deck& deck, bool s) : piles(PILES), rules() {
    if (s)
        deck.shuffle();
    deal(deck);
}

void ScorpionGame::deal(Deck& d) {
    for_all_piles([&] (unsigned i, Pile& p) {
        if (i < TURNED_PILES) {
            p = d.deal(2, 5);
        } else {
            p = d.deal(7, true);
        }
    });
    stock = d.deal(3, false);
}

bool ScorpionGame::operator==(const Game& g) const {
    try {
        const ScorpionGame& sg = dynamic_cast<const ScorpionGame&>(g);
        bool e = true;
        for_all_piles([&] (unsigned i, const Pile& p) {
            e &= std::equal(p.begin(), p.end(), sg.piles[i].begin());
        });
        return e && std::equal(stock.begin(), stock.end(), sg.stock.begin());
    } catch (const std::bad_cast& e) {
        return false;
    }
}

bool ScorpionGame::empty() const {
    bool empty = true;
    for_all_piles([&] (unsigned i, const Pile& p) {
        empty &= p.empty();
    });
    return empty && stock.empty();
}

GameState ScorpionGame::state() const {
    GameState state;
    for_all_piles([&] (unsigned i, const Pile& p) {
        std::for_each(p.begin(), p.end(), [&] (const Card& c) {
            state.push_front(c.get());
        });
        state.push_front(Card::card_separator().get());
    });
    std::for_each(stock.begin(), stock.end(), [&] (const Card& c) {
        state.push_front(c.get());
    });
    return state;
}

void ScorpionGame::state(GameState& gs) {
    stock.clear();
    CardCode cc;
    while (!gs.empty() && !Card::separator(cc = gs.front())) {
        stock.push_front(Card(cc));
        gs.pop_front();
    }
    if (!gs.empty())
        gs.pop_front();
    std::for_each(piles.rbegin(), piles.rend(), [&] (Pile& p) {
        p.clear();
        while (!gs.empty() && !Card::separator(cc = gs.front())) {
            p.push_front(Card(cc));
            gs.pop_front();
        }
        if (!gs.empty())
            gs.pop_front();
    });
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

std::list<ScorpionStep> ScorpionGame::valid_steps() const {
    std::list<ScorpionStep> steps;
    if (is_four_pile_all_ace()) {
        return steps;
    }

    for_all_piles([&] (unsigned from, const Pile& p_from) {
        for_all_piles([&] (unsigned to, const Pile& p_to) {
            if (from == to)
                return;
            auto prev = p_from.begin();
            for (auto c = p_from.begin(); c != p_from.end(); ++c) {
                // upturn if previous step moved pile from downturned card
                if (!c->upturned()) {
                    if (*prev == steps.back().card) {
                        steps.back().turnedup = true;
                    }
                    prev = c;
                    continue;
                }
                // normal step
                if (!p_to.empty() && rules.is_before(*c, p_to.front())) {
                    steps.push_back(ScorpionStep(from, to, *c));
                }
                // king step
                if (p_to.empty() && c->rank() == KING) {
                    // don't move already king topped pile after stock move, makes no difference
                    if (stock.empty() && p_from.back() == *c)
                        continue;
                    steps.push_back(ScorpionStep(from, to, *c));
                }
                prev = c;
            }
        });
    });
    // stock move
    if (!stock.empty()) {
        steps.push_back(ScorpionStep(STOCK_MOVE, STOCK_MOVE, stock.front()));
    }
    return steps;
}

bool ScorpionGame::win() const {
    if (!stock.empty())
        return false;
    for (auto p = piles.begin(); p != piles.end(); ++p) {
        if (p->size() > 0 && p->size() < 13)
            return false;
        if (p->empty())
            continue;
        if (p->front().rank() != ACE)
            return false;

        auto card = p->begin();
        auto prev = card++;
        do {
            if (!rules.is_before(*prev, *card))
                return false;
            prev = card;
        } while (++card != p->end());
    }
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
            if (s.turnedup) {
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

bool ScorpionGame::is_four_pile_all_ace() const {
    int n_pile = 0, n_ace = 0;
    for_all_piles([&] (unsigned i, const Pile& p) {
        if (!p.empty()) {
            n_pile++;
            n_ace += (p.front().rank() == ACE) ? 1 : 0;
        }
    });
    return (n_pile == 4 && n_ace == 4 && stock.empty());
}

std::ostream& operator<<(std::ostream& os, const ScorpionGame& g) {
    g.for_all_piles([&] (unsigned i, const Pile& p) {
        os << p << std::endl;
    });
    os << g.stock;
    return os;
}

std::istream& operator>>(std::istream& is, ScorpionGame& g) {
    std::string line;
    g.for_all_piles([&] (unsigned i, Pile& p) {
        std::getline(is, line);
        // std::cout << "line:" << line << "e" << line.empty() << std::endl;
        std::stringstream ss(line);
        ss >> p;
    });
    std::getline(is, line);
    std::stringstream ss(line);
    ss >> g.stock;
    return is;
}
