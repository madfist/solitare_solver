#include "ScorpionStep.hpp"

const uint32_t ScorpionStep::STOCK_MOVE = 0x80000000;

const std::array<uint32_t, 7> ScorpionStep::MASKS = {0x000000FF,0x00000700,0x00003800,0x00004000,0x003F0000,0x3F000000,0x80000000};

const std::array<uint32_t, 7> ScorpionStep::SHIFTS = {0, 8, 11, 14, 16, 24, 31};

ScorpionStep::ScorpionStep(uint32_t d) : SingleVectorPileStep(d) {}

ScorpionStep::ScorpionStep(
        CardCode cc, unsigned fr, unsigned to,
        unsigned p1, unsigned p2) : SingleVectorPileStep(0) {
    uint32_t data = 0;
    data += cc << shift(SingleVectorPileStep::CARD_CODE);
    data += fr << shift(SingleVectorPileStep::PILE_FROM);
    data += to << shift(SingleVectorPileStep::PILE_TO);
    data += p1 << shift(SingleVectorPileStep::CARD_POS);
    data += p2 << shift(SingleVectorPileStep::NEW_POS);
    init(data);
}

uint32_t ScorpionStep::mask(MaskElement m) const {
    return MASKS[m];
}

uint32_t ScorpionStep::shift(MaskElement m) const {
    return SHIFTS[m];
}
