#include <algorithm>
#include <random>
#include <sstream>
#include <iostream>

#include "Deck.hpp"

const unsigned Deck::DECK_SIZE = 52;
const unsigned Deck::SUITE_SIZE = 4;
const unsigned Deck::RANK_SIZE = 13;

Deck::Deck(bool ut) : deck(DECK_SIZE), cards_left(deck.rbegin()) {
    for (int r = 0; r < RANK_SIZE; ++r) {
        for (int s = 0; s < SUITE_SIZE; ++s) {
            deck[s*RANK_SIZE + r].rank(r).suite(s).turnup(ut);
        }
    }
}

void Deck::shuffle() {
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(time(NULL)));
}

Pile Deck::deal(int n, bool ut) {
    Pile pile(std::make_move_iterator(cards_left), std::make_move_iterator(cards_left + n));
    pile.reverse();
    cards_left += n;
    for (auto i = pile.begin(); i != pile.end(); ++i) {
        i->turnup(ut);
    }
    return pile;
}

Pile Deck::deal(int nd, int nu) {
    auto pile = deal(nd + nu, true);
    int c = 0;
    for (auto i = pile.rbegin(); i != pile.rend() && c < nd; ++i, ++c) {
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
        for (auto c = p.rbegin(); c != p.rend(); ++c) {
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
    if (is.peek() == '\r')
        is.get();
    if (is.peek() == EOF)
        is.get();
    while (!is.eof() && is.peek() != '\r') {
        is >> c;
        p.push_front(c);
    }
    return is;
}