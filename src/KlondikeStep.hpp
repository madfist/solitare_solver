#ifndef SOLITARE_SOLVER_KLONDIKE_STEP_HEADER
#define SOLITARE_SOLVER_KLONDIKE_STEP_HEADER

#include <array>

#include "Card.hpp"
#include "SingleVectorPileStep.hpp"

/*
 * Step information
 *
 * 32bit: [0-7 CardCode][8-11 From][12-15 To][16 TurnedUp][17-22 CardPos][23][24-29 NewCardPos][30][31 StockMove]
 */
class KlondikeStep : public SingleVectorPileStep {
public:
    KlondikeStep(uint32_t);
    KlondikeStep(CardCode, unsigned, unsigned, unsigned, unsigned, bool s = false);

    static const uint32_t STOCK_MOVE;
private:
    virtual uint32_t mask(MaskElement) const override;
    virtual uint32_t shift(MaskElement) const override;

    static const std::array<uint32_t, 7> MASKS;
    static const std::array<uint32_t, 7> SHIFTS;
};

#endif