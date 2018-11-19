#include "Card.hpp"
#include <iostream>

char Card::card_strings[] = {'S','C','H','D','A','2','3','4','5','6','7','8','9','T','J','Q','K','_','^','#'};

const std::array<uint8_t, 4> Card::MASKS = {0x0F, 0x30, 0x40, 0x80};
const std::array<uint8_t, 4> Card::SHIFTS = {0, 4, 6, 7};

static const CardCode CARD_STRINGS_SUITE_SHIFT = 4;
static const CardCode CARD_STRINGS_TURN_SHIFT = 17;

const CardCode Card::CARD_SEPARATOR = 0x80;

Card::Card() : VersatileMask(CARD_SEPARATOR) {}

Card::Card(CardCode c) : VersatileMask(c) {}

Card::Card(const std::string& cs) : VersatileMask(CARD_SEPARATOR) {
    parse(cs);
}

Card::~Card() {}

Card& Card::operator=(const CardCode& cc) {
    set(cc);
    return *this;
}

CardCode Card::rank() const {
    return get(CardMaskElement::RANK);
}

Card& Card::rank(CardCode c) {
    set(CardMaskElement::RANK, c);
    return *this;
}

CardCode Card::suite() const {
    return get(CardMaskElement::SUITE);
}

Card& Card::suite(CardCode c) {
    set(CardMaskElement::SUITE, c);
    return *this;
}

Card& Card::turnup(bool ut) {
    set(CardMaskElement::UPTURNED, ut);
    return *this;
}

CardCode& Card::turnup(CardCode& cc, bool ut) {
    cc = Card(cc).turnup(ut).get();
    return cc;
}

Card::operator bool() const {
    return get(CardMaskElement::UPTURNED);
}

Card::operator CardCode() const {
    return get();
}

void Card::parse(const std::string& cs) {
    if (cs[0] == '#') {
        set(CARD_SEPARATOR);
        return;
    }
    set(CardMaskElement::SUITE, search_card(cs[0]));
    set(CardMaskElement::RANK, search_card(cs[1]) - CARD_STRINGS_SUITE_SHIFT);
    set(CardMaskElement::UPTURNED, search_card(cs[2]) - CARD_STRINGS_TURN_SHIFT);
    set(CardMaskElement::SEPARATOR, 0);
}

std::string Card::print() const {
    if (get(CardMaskElement::SEPARATOR)) {
        return "#";
    }
    return std::string({
        card_strings[get(CardMaskElement::SUITE)],
        card_strings[get(CardMaskElement::RANK) + CARD_STRINGS_SUITE_SHIFT],
        card_strings[get(CardMaskElement::UPTURNED) + CARD_STRINGS_TURN_SHIFT]
    });
}

CardCode Card::search_card(char c) {
    CardCode i = 0;
    while (i < 20 && c != card_strings[i++]);
    return i%20-1;
}

uint8_t Card::mask(CardMaskElement m) const {
    return MASKS[m];
}

uint8_t Card::shift(CardMaskElement m) const {
    return SHIFTS[m];
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