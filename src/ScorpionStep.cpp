#include <iostream>

#include "ScorpionStep.hpp"

const uint32_t ScorpionStep::STOCK_MOVE     = 0x80000000;
const uint32_t ScorpionStep::CARD_CODE_MASK = 0x000000FF;
const uint32_t ScorpionStep::PILE_FROM_MASK = 0x00000700;
const uint32_t ScorpionStep::PILE_TO_MASK   = 0x00003800;
const uint32_t ScorpionStep::TURNED_UP_MASK = 0x00004000;
const uint32_t ScorpionStep::CARD_POS_MASK  = 0x003F0000;
const uint32_t ScorpionStep::NEW_POS_MASK   = 0x3F000000;

const uint32_t ScorpionStep::CARD_CODE_SHIFT =  0;
const uint32_t ScorpionStep::PILE_FROM_SHIFT =  8;
const uint32_t ScorpionStep::PILE_TO_SHIFT   = 11;
const uint32_t ScorpionStep::TURNED_UP_SHIFT = 14;
const uint32_t ScorpionStep::CARD_POS_SHIFT  = 16;
const uint32_t ScorpionStep::NEW_POS_SHIFT   = 24;

ScorpionStep::ScorpionStep(uint32_t d) : data(d) {}

ScorpionStep::ScorpionStep(
        CardCode cc, unsigned fr, unsigned to,
        unsigned p1, unsigned p2) : data(0) {
    data += cc << CARD_CODE_SHIFT;
    data += fr << PILE_FROM_SHIFT;
    data += to << PILE_TO_SHIFT;
    data += p1 << CARD_POS_SHIFT;
    data += p2 << NEW_POS_SHIFT;
}

bool ScorpionStep::is_stock_move() const {
    return (data == STOCK_MOVE);
}

CardCode ScorpionStep::card_code() const {
    return (data & CARD_CODE_MASK) >> CARD_CODE_SHIFT;
}

unsigned ScorpionStep::pile_from() const {
    return (data & PILE_FROM_MASK) >> PILE_FROM_SHIFT;
}

unsigned ScorpionStep::pile_to() const {
    return (data & PILE_TO_MASK) >> PILE_TO_SHIFT;
}

bool ScorpionStep::turned_up() const {
    return (data & TURNED_UP_MASK) >> TURNED_UP_SHIFT;
}

unsigned ScorpionStep::card_pos() const {
    return (data & CARD_POS_MASK) >> CARD_POS_SHIFT;
}

unsigned ScorpionStep::new_pos() const {
    return (data & NEW_POS_MASK) >> NEW_POS_SHIFT;
}

void ScorpionStep::turned_up(bool ut) {
    data &= ~TURNED_UP_MASK;
    data |= (ut) ? 1 << TURNED_UP_SHIFT : 0;
}

std::ostream& operator<<(std::ostream& os, const ScorpionStep& s) {
    if (s.data == ScorpionStep::STOCK_MOVE) {
        os << "STOCK_MOVE";
        return os;
    }
    os << "[" << Card(s.card_code()) << ":"  << s.pile_from() + 1 << "->" << s.pile_to() + 1 << "]";
    os << "{" << s.turned_up() << ", " << s.card_pos() << ", " << s.new_pos() << "}";
    os << "(" << std::hex << s.data << std::dec << ")";
    return os;
}