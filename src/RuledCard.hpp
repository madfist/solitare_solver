#ifndef RULED_CARD_HEADER
#define RULED_CARD_HEADER

#include "Card.hpp"
#include "Rules.hpp"

class RuledCard {
public:
    RuledCard() = delete;
    RuledCard(const Card&, const Rules&);
    RuledCard(const CardCode&, const Rules&);

    bool operator<(const Card&);
    bool operator>(const Card&);
private:
    Card card;
    Rules rules;
};

#endif