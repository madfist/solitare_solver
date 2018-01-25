#include <algorithm>
#include <random>
#include <sstream>
#include <iostream>

#include "Deck.hpp"

static const int DECK_SIZE = 52;
static const int SUITE_SIZE = 4;
static const int RANK_SIZE = 13;

Deck::Deck() : deck(DECK_SIZE), cards_left(deck.rbegin()) {
    for (int r = 0; r < RANK_SIZE; ++r) {
        for (int s = 0; s < SUITE_SIZE; ++s) {
            deck[s*RANK_SIZE + r].rank(r);
            deck[s*RANK_SIZE + r].suite(s);
        }
    }
}

void Deck::shuffle() {
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(time(NULL)));
}

Pile Deck::deal(int n, bool ut) {
    Pile pile(std::make_move_iterator(cards_left), std::make_move_iterator(cards_left + n));
    cards_left += n;
    for (auto i = pile.begin(); i != pile.end(); ++i) {
        i->turnup(ut);
    }
    return pile;
}

Pile Deck::deal(int nd, int nu) {
    auto pile = deal(nd + nu, true);
    int c = 0;
    for (auto i = pile.begin(); i != pile.end() && c < nd; ++i, ++c) {
        i->turnup(false);
    }
    return pile;
}

std::ostream& operator<<(std::ostream& os, const Deck& d) {
    for (int i = 0; i < d.deck.size(); ++i) {
        os << d.deck[i];
    }
    return os;
}

std::istream& operator>>(std::istream& is, Deck& d) {
    int i = 0;
    while (!is.eof() || i < d.deck.size()) {
        is >> d.deck[i++];
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Pile& p) {
    if (!p.empty()) {
        int i = 0;
        for (auto c = p.begin(); c != p.end(); ++c) {
            os << *c;
            if (i++ < p.size()-1) {
                os << ' ';
            }
        }
    }
    return os;
}

std::istream& operator>>(std::istream& is, Pile& p) {
    Card c;
    p.clear();
    // std::cout << "peek" << is.peek() << std::endl;
    if (is.peek() == EOF) {
        // std::cout << "EOF" << std::endl;
        is.get();
    }
    while (!is.eof()) {
        is >> c;
        p.push_back(c);
    }
    return is;
}