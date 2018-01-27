#ifndef SOLITARE_SOLVER_SCORPION_STEP_HEADER
#define SOLITARE_SOLVER_SCORPION_STEP_HEADER

#include "Card.hpp"

/*
 * Step information
 *
 * 32bit: [0-7 CardCode][8-10 From][11-13 To][14 TurnedUp][15][16-21 CardPos][22-23][24-29 NewCardPos][30][31 StockMove]
 */
class ScorpionStep {
public:
    ScorpionStep(uint32_t);
    ScorpionStep(CardCode, unsigned, unsigned, unsigned, unsigned);

    bool is_stock_move() const;
    CardCode card_code() const;
    unsigned pile_from() const;
    unsigned pile_to() const;
    bool turned_up() const;
    unsigned card_pos() const;
    unsigned new_pos() const;

    void turned_up(bool);

    static const uint32_t STOCK_MOVE;

    friend std::ostream& operator<<(std::ostream&, const ScorpionStep&);
private:
    static const uint32_t CARD_CODE_MASK;
    static const uint32_t PILE_FROM_MASK;
    static const uint32_t PILE_TO_MASK;
    static const uint32_t TURNED_UP_MASK;
    static const uint32_t CARD_POS_MASK;
    static const uint32_t NEW_POS_MASK;

    static const uint32_t CARD_CODE_SHIFT;
    static const uint32_t PILE_FROM_SHIFT;
    static const uint32_t PILE_TO_SHIFT;
    static const uint32_t TURNED_UP_SHIFT;
    static const uint32_t CARD_POS_SHIFT;
    static const uint32_t NEW_POS_SHIFT;

    uint32_t data;
};

#endif