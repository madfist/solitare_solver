#include "RuledCard.hpp"

RuledCard::RuledCard(const Card& c, const Rules& r) : card(c), rules(r) {}
RuledCard::RuledCard(const CardCode& cc, const Rules& r) : card(cc), rules(r) {}

bool RuledCard::operator<(const Card& c) {
    rules.is_before(card, c);
}

bool RuledCard::operator>(const Card& c) {
    rules.is_before(c, card);
}