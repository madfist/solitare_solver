#include "RuledCard.hpp"

RuledCard::RuledCard(const Card& c, const Rules& r) : card(c), rules(r) {}
RuledCard::RuledCard(const CardCode& cc, const Rules& r) : card(cc), rules(r) {}

bool RuledCard::operator<(const Card& c) {
    return rules.is_before(card, c);
}

bool RuledCard::operator>(const Card& c) {
    return rules.is_before(c, card);
}

bool RuledCard::operator<(const CardCode& cc) {
    return rules.is_before(card, Card(cc));
}

bool RuledCard::operator>(const CardCode& cc) {
    return rules.is_before(Card(cc), card);
}