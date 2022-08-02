/**
 * Single-vector pile step
 * @file SingleVectorPileStep.hpp
 * @author Adam Koleszar
 */
#ifndef SOLITARE_SOLVER_SINGLE_VECTOR_PILE_STEP_HEADER
#define SOLITARE_SOLVER_SINGLE_VECTOR_PILE_STEP_HEADER

#include <vector>
#include <algorithm>
#include <iostream>

#include "Card.hpp"
#include "VersatileMask.hpp"
#include "PrettyPrint.hpp"

/// Enum for a pile step bitmask
enum StateMaskElement {
    CARD_CODE = 0, PILE_FROM, PILE_TO, TURNED_UP, CARD_POS, NEW_POS, STOCK_STEP, WEIGHT
};

/**
 * @brief Class for representing steps in a game as a bitmask
 * @details it holds the following information
 * * card to move
 * * pile to move from
 * * pile to move to
 * * turning up the card on top
 * * doing a stock move
 * * original card position in source pile
 * * new card position in destination pile
 * * weight of step (for heuristic solving)
 */
class SingleVectorPileStep : public VersatileMask<uint32_t, StateMaskElement> {
public:
    SingleVectorPileStep(uint32_t);
    virtual ~SingleVectorPileStep();

    using Hash = VersatileMaskHash<uint32_t, StateMaskElement>;

    bool is_stock_move() const;
    CardCode card_code() const;
    unsigned pile_from() const;
    unsigned pile_to() const;
    bool turned_up() const;
    unsigned card_pos() const;
    unsigned new_pos() const;
    unsigned weight() const;

    void turned_up(bool); ///< Change upturned state

    bool operator<(const SingleVectorPileStep&) const; ///< Order steps by weight or starting pile

    /**
     * @brief Parse step from text
     * @details Text have the following
     * * starts with STOCK_MOVE if needed
     * * card as text (e.g. `CK_`)
     * * `:PILE_FROM:CARD_POS`
     * * `->:PILE_TO:NEW_POS`
     * * upturned state `[_^]`
     * * weight
     */
    void parse(const std::string&);

    friend std::ostream& operator<<(std::ostream&, const SingleVectorPileStep&); ///< Stream output operator
    friend std::istream& operator>>(std::istream&, SingleVectorPileStep&);       ///< Stream input operator
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

/// Stream output operator for steps
template<class Step>
std::ostream& operator<<(std::ostream& os, const std::vector<Step>& ss) {
    for (auto s : ss) {
        os << s << ' ';
    }
    os << ss.back();
    return os;
}

template<class Output, class Step>
Output& operator<<(PrettyPrintWrapper<Output> ppw, const std::vector<Step>& ss) {
    for (auto s : ss) {
        ppw << s << ' ';
    }
    ppw << ss.back();
    return ppw.output;
}

/// Stream input operator for steps
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
