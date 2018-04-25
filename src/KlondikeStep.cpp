#include "KlondikeStep.hpp"

const uint32_t KlondikeStep::STOCK_MOVE = 0x80000000;

const std::array<uint32_t, 7> KlondikeStep::MASKS = {0x000000FF,0x00000F00,0x0000F000,0x00010000,0x007E0000,0x3F000000,0x80000000};

const std::array<uint32_t, 7> KlondikeStep::SHIFTS = {0, 8, 12, 16, 17, 24, 31};

KlondikeStep::KlondikeStep(uint32_t d) : SingleVectorPileStep(d) {}

KlondikeStep::KlondikeStep(
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

uint32_t KlondikeStep::mask(MaskElement m) const {
    return MASKS[m];
}

uint32_t KlondikeStep::shift(MaskElement m) const {
    return SHIFTS[m];
}