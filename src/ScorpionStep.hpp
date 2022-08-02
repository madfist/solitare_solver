#ifndef SOLITARE_SOLVER_SCORPION_STEP_HEADER
#define SOLITARE_SOLVER_SCORPION_STEP_HEADER

#include <array>

#include "Card.hpp"
#include "SingleVectorPileStep.hpp"

/*
 * Step information
 *
 * 32bit: [0-7 CardCode][8-10 From][11-13 To][14 TurnedUp][15-21 CardPos][22-28 NewCardPos][29 StockMove][30-31 Weight]
 */
class ScorpionStep : public SingleVectorPileStep {
public:
    /// Create step from mask
    ScorpionStep(uint32_t);
    /**
     * @brief Create step from moving data
     * @param cc card code
     * @param fr source pile
     * @param to destination pile
     * @param p1 position in source pile
     * @param p2 position in destination pile
     * @param w optional weight for the solver to sort possible steps (default: 0)
     */
    ScorpionStep(CardCode cc, unsigned fr, unsigned to, unsigned p1, unsigned p2, unsigned w = 0);

    static const uint32_t STOCK_MOVE;  ///< Constant for the stock move
private:
    virtual uint32_t mask(StateMaskElement) const override;
    virtual uint8_t shift(StateMaskElement) const override;

    static const std::array<uint32_t, 8> MASKS;
    static const std::array<uint8_t, 8> SHIFTS;
};

#endif
