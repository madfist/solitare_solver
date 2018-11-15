#include <ostream>

#include "SingleVectorPileStep.hpp"

SingleVectorPileStep::SingleVectorPileStep(uint32_t d) : data(d) {}

bool SingleVectorPileStep::is_stock_move() const {
    return (data & mask(STOCK_STEP));
}

CardCode SingleVectorPileStep::card_code() const {
    return (data & mask(CARD_CODE)) >> shift(CARD_CODE);
}

unsigned SingleVectorPileStep::pile_from() const {
    return (data & mask(PILE_FROM)) >> shift(PILE_FROM);
}

unsigned SingleVectorPileStep::pile_to() const {
    return (data & mask(PILE_TO)) >> shift(PILE_TO);
}

bool SingleVectorPileStep::turned_up() const {
    return (data & mask(TURNED_UP)) >> shift(TURNED_UP);
}

unsigned SingleVectorPileStep::card_pos() const {
    return (data & mask(CARD_POS)) >> shift(CARD_POS);
}

unsigned SingleVectorPileStep::new_pos() const {
    return (data & mask(NEW_POS)) >> shift(NEW_POS);
}

void SingleVectorPileStep::turned_up(bool ut) {
    data &= ~mask(TURNED_UP);
    data |= (ut) ? 1 << shift(TURNED_UP) : 0;
}

void SingleVectorPileStep::init(uint32_t d) {
    data = d;
}

std::ostream& operator<<(std::ostream& os, const SingleVectorPileStep& s) {
    if (s.is_stock_move()) {
        os << "STOCK_MOVE";
        if (!s.card_code())
            return os;
    }
    os << "[" << Card(s.card_code()) << ":"  << s.pile_from() + 1 << "->" << s.pile_to() + 1 << "]";
    // os << "{" << s.turned_up() << ", " << s.card_pos() << ", " << s.new_pos() << "}";
    // os << "(" << std::hex << s.data << std::dec << ")";
    return os;
}
