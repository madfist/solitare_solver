#ifndef SOLITARE_SOLVER_SINGLE_VECTOR_PILE_STEP_HEADER
#define SOLITARE_SOLVER_SINGLE_VECTOR_PILE_STEP_HEADER

#include <vector>
#include <algorithm>
#include <iostream>

#include "Card.hpp"

class SingleVectorPileStep {
public:
    SingleVectorPileStep(uint32_t);

    bool is_stock_move() const;
    CardCode card_code() const;
    unsigned pile_from() const;
    unsigned pile_to() const;
    bool turned_up() const;
    unsigned card_pos() const;
    unsigned new_pos() const;

    void turned_up(bool);

    friend std::ostream& operator<<(std::ostream&, const SingleVectorPileStep&);
protected:
    enum MaskElement {
        CARD_CODE = 0, PILE_FROM, PILE_TO, TURNED_UP, CARD_POS, NEW_POS, STOCK_STEP
    };
    void init(uint32_t);
private:
    virtual uint32_t mask(MaskElement) const = 0;
    virtual uint32_t shift(MaskElement) const = 0;

    uint32_t data;
};

template<class Step>
std::ostream& operator<<(std::ostream& os, const std::vector<Step>& ss) {
    std::for_each(ss.begin(), ss.end() - 1, [&] (const Step& s) {
        os << s << '\n';
    });
    os << ss.back();
    return os;
}

#endif