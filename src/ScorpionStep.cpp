#include "ScorpionStep.hpp"

const uint32_t ScorpionStep::STOCK_MOVE = 0x20000000;

const std::array<uint32_t, 8> ScorpionStep::MASKS = {0x000000FF,0x00000700,0x00003800,0x00004000,0x003F8000,0x1FC00000,0x20000000,0xC0000000};

const std::array<uint8_t, 8> ScorpionStep::SHIFTS = {0, 8, 11, 14, 15, 22, 29, 30};

ScorpionStep::ScorpionStep(uint32_t d) : SingleVectorPileStep(d) {}

ScorpionStep::ScorpionStep(
        CardCode cc, unsigned fr, unsigned to,
        unsigned p1, unsigned p2, unsigned w) : SingleVectorPileStep(0) {
    set(StateMaskElement::CARD_CODE, cc);
    set(StateMaskElement::PILE_FROM, fr);
    set(StateMaskElement::PILE_TO, to);
    set(StateMaskElement::CARD_POS, p1);
    set(StateMaskElement::NEW_POS, p2);
    set(StateMaskElement::WEIGHT, w);
}

uint32_t ScorpionStep::mask(StateMaskElement m) const {
    return MASKS[m];
}

uint8_t ScorpionStep::shift(StateMaskElement m) const {
    return SHIFTS[m];
}
