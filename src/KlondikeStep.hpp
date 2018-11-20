#ifndef SOLITARE_SOLVER_KLONDIKE_STEP_HEADER
#define SOLITARE_SOLVER_KLONDIKE_STEP_HEADER

#include <array>

#include "Card.hpp"
#include "SingleVectorPileStep.hpp"

/*
 * Step information
 *
 * 32bit: [0-7 CardCode][8-11 From][12-15 To][16 TurnedUp][17-22 CardPos][23-28 NewCardPos][29 StockMove][30-31 Weight]
 */
class KlondikeStep : public SingleVectorPileStep {
public:
    KlondikeStep(uint32_t);
    KlondikeStep(CardCode, unsigned, unsigned, unsigned, unsigned, unsigned w = 0, bool s = false);

    static const uint32_t STOCK_MOVE;

    KlondikeStep operator-() const;
private:
    virtual uint32_t mask(StateMaskElement) const override;
    virtual uint8_t shift(StateMaskElement) const override;

    static const std::array<uint32_t, 8> MASKS;
    static const std::array<uint8_t, 8> SHIFTS;
};

#endif