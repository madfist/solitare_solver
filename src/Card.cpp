#include "Card.hpp"
#include <iostream>

char Card::card_strings[] = {'S','C','H','D','A','2','3','4','5','6','7','8','9','T','J','Q','K','_','^','#'};

static const CardCode CARD_STRINGS_SUITE_SHIFT = 4;
static const CardCode CARD_STRINGS_TURN_SHIFT = 17;
static const CardCode CARD_RANK = 0x0F;
static const CardCode CARD_SUITE = 0x30;
static const CardCode CARD_UPTURNED = 0x40;
static const CardCode CARD_SUITE_SHIFT = 4;
static const CardCode CARD_TURN_SHIFT = 6;

const CardCode Card::CARD_SEPARATOR = 0x80;

Card::Card() : card(CARD_SEPARATOR) {}

Card::Card(CardCode c) : card(c) {}

Card::Card(const std::string& cs) : card(CARD_SEPARATOR) {
    parse(cs);
}

Card::Card(const Card& c) {
    card = c.card;
}

Card& Card::operator=(const Card& c) {
    card = c.card;
    return *this;
}

Card& Card::operator=(const CardCode& cc) {
    card = cc;
    return *this;
}

CardCode Card::get() const {
    return card;
}

Card& Card::set(CardCode c) {
    card = c;
    return *this;
}

CardCode Card::rank() const {
    return card & CARD_RANK;
}

Card& Card::rank(CardCode c) {
    card &= ~CARD_SEPARATOR;
    card &= ~CARD_RANK;
    card |= c;
    return *this;
}

CardCode Card::suite() const {
    return (card & CARD_SUITE) >> CARD_SUITE_SHIFT;
}

Card& Card::suite(CardCode c) {
    card &= ~CARD_SEPARATOR;
    card &= ~CARD_SUITE;
    card |= (c << CARD_SUITE_SHIFT);
    return *this;
}

bool Card::upturned() const {
    return (card & CARD_UPTURNED);
}

Card& Card::turnup(bool ut) {
    card &= ~CARD_SEPARATOR;
    card &= ~CARD_UPTURNED;
    card |= (ut) ? 1 << CARD_TURN_SHIFT : 0;
    return *this;
}

bool Card::upturned(CardCode cc) {
    return (cc & CARD_UPTURNED);
}

CardCode& Card::turnup(CardCode& cc, bool ut) {
    cc &= ~CARD_SEPARATOR;
    cc &= ~CARD_UPTURNED;
    cc |= (ut) ? 1 << CARD_TURN_SHIFT : 0;
    return cc;
}

bool Card::operator==(const Card& c) const {
    return (c.card == card);
}

bool Card::operator!=(const Card& c) const {
    return (c.card != card);
}

Card::operator bool() const {
    return (card & CARD_SEPARATOR);
}

Card::operator CardCode() const {
    return card;
}

void Card::parse(const std::string& cs) {
    if (cs[0] == '#') {
        card = CARD_SEPARATOR;
        return;
    }
    card = search_card(cs[0]) << CARD_SUITE_SHIFT;
    card += search_card(cs[1]) - CARD_STRINGS_SUITE_SHIFT;
    card += (search_card(cs[2]) - CARD_STRINGS_TURN_SHIFT) << CARD_TURN_SHIFT;
}

std::string Card::print() const {
    if (card == CARD_SEPARATOR)
        return "#";
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

std::ostream& operator<<(std::ostream& os, const CardCode& cc) {
    os << Card(cc) << "[" << (unsigned)cc << "]";
    return os;
}