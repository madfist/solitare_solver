#ifndef SOLITARE_SOLVER_SINGLE_VECTOR_PILE_STEP_HEADER
#define SOLITARE_SOLVER_SINGLE_VECTOR_PILE_STEP_HEADER

#include <vector>
#include <algorithm>
#include <iostream>

#include "Card.hpp"
#include "VersatileMask.hpp"
#include "PrettyPrint.hpp"

enum StateMaskElement {
    CARD_CODE = 0, PILE_FROM, PILE_TO, TURNED_UP, CARD_POS, NEW_POS, STOCK_STEP, WEIGHT
};

class SingleVectorPileStep : public VersatileMask<uint32_t, StateMaskElement> {
public:
    SingleVectorPileStep(uint32_t);
    virtual ~SingleVectorPileStep();

    bool is_stock_move() const;
    CardCode card_code() const;
    unsigned pile_from() const;
    unsigned pile_to() const;
    bool turned_up() const;
    unsigned card_pos() const;
    unsigned new_pos() const;
    unsigned weight() const;

    void turned_up(bool);

    bool operator<(const SingleVectorPileStep&) const;

    void parse(const std::string&);

    friend std::ostream& operator<<(std::ostream&, const SingleVectorPileStep&);
    friend std::istream& operator>>(std::istream&, SingleVectorPileStep&);
    template<class Output>
    friend Output& operator<<(PrettyPrintWrapper<Output>, const SingleVectorPileStep&);
private:
    virtual uint32_t mask(StateMaskElement) const = 0;
    virtual uint8_t shift(StateMaskElement) const = 0;
};

template<class Output>
Output& operator<<(PrettyPrintWrapper<Output> ppw, const SingleVectorPileStep& s) {
    if (s.is_stock_move()) {
        ppw.output<< "STOCK_MOVE";
        if (!s.card_code())
            return ppw.output;
    }
    ppw.output << "[" << Card(s.card_code()) << ":"  << s.pile_from() + 1 << "->" << s.pile_to() + 1 << "]" << s.weight() << ";";
    return ppw.output;
}

template<class Step>
std::ostream& operator<<(std::ostream& os, const std::vector<Step>& ss) {
    std::for_each(ss.begin(), ss.end() - 1, [&] (const Step& s) {
        os << s << ' ';
    });
    os << ss.back();
    return os;
}

template<class Step>
std::istream& operator>>(std::istream& is, std::vector<Step>& ss) {
    Step s(0);
    while(!is.eof()) {
        is >> s;
        ss.push_back(s);
    }
    return is;
}

#endif