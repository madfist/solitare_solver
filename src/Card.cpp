#include "Card.hpp"
#include <iostream>

char Card::card_strings[] = {'S','C','H','D','A','2','3','4','5','6','7','8','9','T','J','Q','K','_','^','X'};

static const CardCode CARD_STRINGS_SUITE_SHIFT = 4;
static const CardCode CARD_STRINGS_TURN_SHIFT = 17;
static const CardCode CARD_RANK = 0x0F;
static const CardCode CARD_SUITE = 0x30;
static const CardCode CARD_UPTURNED = 0x40;
static const CardCode CARD_SEPARATOR = 0x80;
static const CardCode CARD_SUITE_SHIFT = 4;
static const CardCode CARD_TURN_SHIFT = 6;

Card::Card() : card(0) {}

Card::Card(CardCode c) : card(c) {}

Card::Card(const std::string& cs) : card(0) {
    parse(cs);
}

Card::Card(const Card& c) {
    card = c.card;
}

Card& Card::operator=(const Card& c) {
    card = c.card;
    return *this;
}

void Card::set(CardCode c) {
    card = c;
}

CardCode Card::rank() const {
    return card & CARD_RANK;
}

void Card::rank(CardCode c) {
    card &= ~CARD_RANK;
    card |= c;
}

CardCode Card::suite() const {
    return (card & CARD_SUITE) >> CARD_SUITE_SHIFT;
}

void Card::suite(CardCode c) {
    card &= ~CARD_SUITE;
    card |= (c << CARD_SUITE_SHIFT);
}

bool Card::upturned() const {
    return (card & CARD_UPTURNED);
}

void Card::turnup(bool ut) {
    card &= ~CARD_UPTURNED;
    card |= (ut) ? 1 << CARD_TURN_SHIFT : 0;
}

bool Card::operator==(const Card& c) const {
    return (c.card == card);
}

bool Card::operator!=(const Card& c) const {
    return (c.card != card);
}

void Card::parse(const std::string& cs) {
    //todo: error check
    card = search_card(cs[0]) << CARD_SUITE_SHIFT;
    card += search_card(cs[1]) - CARD_STRINGS_SUITE_SHIFT;
    card += (search_card(cs[2]) - CARD_STRINGS_TURN_SHIFT) << CARD_TURN_SHIFT;
}

std::string Card::print() const {
    std::string out("___");
    out[0] = card_strings[(card & CARD_SUITE) >> CARD_SUITE_SHIFT];
    out[1] = card_strings[(card & CARD_RANK) + CARD_STRINGS_SUITE_SHIFT];
    out[2] = card_strings[((card & CARD_UPTURNED) >> CARD_TURN_SHIFT) + CARD_STRINGS_TURN_SHIFT];
    return out;
}

CardCode Card::search_card(char c) {
    CardCode i = 0;
    while (i < 20 && c != card_strings[i++]);
    return i%20-1;
}

std::ostream& operator<<(std::ostream& os, const Card& c) {
    os << c.print();
    return os;
}

std::istream& operator>>(std::istream& is, Card& c) {
    std::string in;
    is >> in;
    c.parse(in);
    return is;
}