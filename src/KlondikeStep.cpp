#include "KlondikeStep.hpp"

const uint32_t KlondikeStep::STOCK_MOVE = 0x20000000;

const std::array<uint32_t, 8> KlondikeStep::MASKS = {0x000000FF,0x00000F00,0x0000F000,0x00010000,0x003E0000,0x1F800000,0x20000000,0xC0000000};

const std::array<uint8_t, 8> KlondikeStep::SHIFTS = {0, 8, 12, 16, 17, 23, 29, 30};

KlondikeStep::KlondikeStep(uint32_t d) : SingleVectorPileStep(d) {}

KlondikeStep::KlondikeStep(
        CardCode cc, unsigned fr, unsigned to,
        unsigned p1, unsigned p2, unsigned w, bool s) : SingleVectorPileStep(0) {
    set(StateMaskElement::CARD_CODE, cc);
    set(StateMaskElement::PILE_FROM, fr);
    set(StateMaskElement::PILE_TO, to);
    set(StateMaskElement::CARD_POS, p1);
    set(StateMaskElement::NEW_POS, p2);
    set(StateMaskElement::STOCK_STEP, s);
    set(StateMaskElement::WEIGHT, w);
}

KlondikeStep KlondikeStep::operator-() const {
    KlondikeStep ret(*this);
    ret.set(PILE_FROM, pile_to());
    ret.set(PILE_TO, pile_from());
    ret.set(CARD_POS, new_pos());
    ret.set(NEW_POS, card_pos());
    return ret;
}

uint32_t KlondikeStep::mask(StateMaskElement m) const {
    return MASKS[m];
}

uint8_t KlondikeStep::shift(StateMaskElement m) const {
    return SHIFTS[m];
}
