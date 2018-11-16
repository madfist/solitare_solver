#include "ScorpionStep.hpp"

const uint32_t ScorpionStep::STOCK_MOVE = 0x08000000;

const std::array<uint32_t, 8> ScorpionStep::MASKS = {0x000000FF,0x00000700,0x00003800,0x00004000,0x001F8000,0x07E00000,0x08000000,0xFC000000};

const std::array<uint32_t, 8> ScorpionStep::SHIFTS = {0, 8, 11, 14, 15, 21, 27, 28};

ScorpionStep::ScorpionStep(uint32_t d) : SingleVectorPileStep(d) {}

ScorpionStep::ScorpionStep(
        CardCode cc, unsigned fr, unsigned to,
        unsigned p1, unsigned p2, unsigned w) : SingleVectorPileStep(0) {
    uint32_t data = 0;
    data += cc << shift(SingleVectorPileStep::CARD_CODE);
    data += fr << shift(SingleVectorPileStep::PILE_FROM);
    data += to << shift(SingleVectorPileStep::PILE_TO);
    data += p1 << shift(SingleVectorPileStep::CARD_POS);
    data += p2 << shift(SingleVectorPileStep::NEW_POS);
    data += w  << shift(SingleVectorPileStep::WEIGHT);
    init(data);
}

uint32_t ScorpionStep::mask(MaskElement m) const {
    return MASKS[m];
}

uint32_t ScorpionStep::shift(MaskElement m) const {
    return SHIFTS[m];
}
