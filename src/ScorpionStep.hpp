#ifndef SOLITARE_SOLVER_SCORPION_STEP_HEADER
#define SOLITARE_SOLVER_SCORPION_STEP_HEADER

#include <array>

#include "Card.hpp"
#include "SingleVectorPileStep.hpp"

/*
 * Step information
 *
 * 32bit: [0-7 CardCode][8-10 From][11-13 To][14 TurnedUp][15-20 CardPos][21-26 NewCardPos][27 StockMove][28-31 Weight]
 */
class ScorpionStep : public SingleVectorPileStep {
public:
    ScorpionStep(uint32_t);
    ScorpionStep(CardCode, unsigned, unsigned, unsigned, unsigned, unsigned w = 0);

    static const uint32_t STOCK_MOVE;
private:
    virtual uint32_t mask(MaskElement) const override;
    virtual uint32_t shift(MaskElement) const override;

    static const std::array<uint32_t, 8> MASKS;
    static const std::array<uint32_t, 8> SHIFTS;
};

#endif