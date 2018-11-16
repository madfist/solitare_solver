#include "KlondikeStep.hpp"

const uint32_t KlondikeStep::STOCK_MOVE = 0x20000000;

const std::array<uint32_t, 8> KlondikeStep::MASKS = {0x000000FF,0x00000F00,0x0000F000,0x00010000,0x003E0000,0x1F800000,0x20000000,0xC0000000};

const std::array<uint32_t, 8> KlondikeStep::SHIFTS = {0, 8, 12, 16, 17, 23, 29, 30};

KlondikeStep::KlondikeStep(uint32_t d) : SingleVectorPileStep(d) {}

KlondikeStep::KlondikeStep(
        CardCode cc, unsigned fr, unsigned to,
        unsigned p1, unsigned p2, unsigned w, bool s) : SingleVectorPileStep(0) {
    uint32_t data = 0;
    data += cc << shift(SingleVectorPileStep::CARD_CODE);
    data += fr << shift(SingleVectorPileStep::PILE_FROM);
    data += to << shift(SingleVectorPileStep::PILE_TO);
    data += p1 << shift(SingleVectorPileStep::CARD_POS);
    data += p2 << shift(SingleVectorPileStep::NEW_POS);
    data += (s) ? 1 << shift(SingleVectorPileStep::STOCK_STEP) : 0;
    data += w << shift(SingleVectorPileStep::WEIGHT);
    init(data);
}

uint32_t KlondikeStep::mask(MaskElement m) const {
    return MASKS[m];
}

uint32_t KlondikeStep::shift(MaskElement m) const {
    return SHIFTS[m];
}