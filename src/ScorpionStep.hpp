#ifndef SOLITARE_SOLVER_SCORPION_STEP_HEADER
#define SOLITARE_SOLVER_SCORPION_STEP_HEADER

#include <array>

#include "Card.hpp"
#include "SingleVectorPileStep.hpp"

/*
 * Step information
 *
 * 32bit: [0-7 CardCode][8-10 From][11-13 To][14 TurnedUp][15][16-21 CardPos][22-23][24-29 NewCardPos][30][31 StockMove]
 */
class ScorpionStep : public SingleVectorPileStep {
public:
    ScorpionStep(uint32_t);
    ScorpionStep(CardCode, unsigned, unsigned, unsigned, unsigned);

    static const uint32_t STOCK_MOVE;
private:
    virtual uint32_t mask(MaskElement) const override;
    virtual uint32_t shift(MaskElement) const override;

    static const std::array<uint32_t, 7> MASKS;
    static const std::array<uint32_t, 7> SHIFTS;
};

#endif