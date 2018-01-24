#include "Card.hpp"
#include <iostream>

char Card::card_strings[] = {'S','C','H','D','A','2','3','4','5','6','7','8','9','T','J','Q','K','_','^','X'};

static const unsigned char CARD_RANK = 0x0F;
static const unsigned char CARD_SUITE = 0x30;
static const unsigned char CARD_UPTURNED = 0x40;
static const unsigned char CARD_SEPARATOR = 0x80;

Card::Card(unsigned char c) : card(c) {}

Card::Card(const std::string& cs) : card(0) {
    parse(cs);
}

bool Card::operator==(const Card& c) const {
    return (c.card == card);
}

bool Card::operator!=(const Card& c) const {
    return (c.card != card);
}

void Card::parse(const std::string& cs) {
    //todo: error check
    card = search_card(cs[0]) << 4;
    card += search_card(cs[1]) - 4;
    card += (search_card(cs[2]) - 17) << 6;
}

std::string Card::print() const {
    std::string out("___");
    out[0] = card_strings[(card & CARD_SUITE) >> 4];
    out[1] = card_strings[(card & CARD_RANK) + 4];
    out[2] = card_strings[((card & CARD_UPTURNED) >> 6) + 17];
    return out;
}

unsigned char Card::search_card(char c) {
    unsigned char i = 0;
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

// Card::Card() : suite_(SPADE), rank_(CA), upturned_(true)
// {}

// Card::Card(Suite s, Rank r) : suite_(s), rank_(r), upturned_(true)
// {}

// Card::Card(Suite s, Rank r, bool ut) : suite_(s), rank_(r), upturned_(ut)
// {}

// bool Card::operator==(const Card& card) const {
//     return (suite_ == card.suite_ && rank_ == card.rank_ && upturned_ == card.upturned_);
// }

// unsigned char Card::hash() const {
//     return static_cast<unsigned char>(rank_) + 13 * static_cast<unsigned char>(suite_) + ((upturned_) ? 52 : 0);
// }

// Suite Card::suite() const {
//     return suite_;
// }

// void Card::set_suite(Suite s) {
//     suite_ = s;
// }

// Rank Card::rank() {
//     return rank_;
// }

// void Card::set_rank(Rank r) const {
//     rank_ = r;
// }

// bool Card::is_upturned() const {
//     return upturned_;
// }

// void Card::turn() {
//     upturned_ = !upturned_;
// }

// void Card::turn(bool t) {
//     upturned_ = t;
// }