#include <ostream>

#include "SingleVectorPileStep.hpp"

SingleVectorPileStep::SingleVectorPileStep(uint32_t d) : VersatileMask(d) {}

SingleVectorPileStep::~SingleVectorPileStep() {}

bool SingleVectorPileStep::is_stock_move() const {
    return get(STOCK_STEP);
}

CardCode SingleVectorPileStep::card_code() const {
    return get(CARD_CODE);
}

uint32_t SingleVectorPileStep::pile_from() const {
    return get(PILE_FROM);
}

uint32_t SingleVectorPileStep::pile_to() const {
    return get(PILE_TO);
}

bool SingleVectorPileStep::turned_up() const {
    return get(TURNED_UP);
}

uint32_t SingleVectorPileStep::card_pos() const {
    return get(CARD_POS);
}

uint32_t SingleVectorPileStep::new_pos() const {
    return get(NEW_POS);
}

uint32_t SingleVectorPileStep::weight() const {
    return get(WEIGHT);
}

void SingleVectorPileStep::turned_up(bool ut) {
    set(TURNED_UP, ut);
}

bool SingleVectorPileStep::operator<(const SingleVectorPileStep& s) const {
    // sorry about this but I need descending order on weights
    return weight() > s.weight();
}

std::ostream& operator<<(std::ostream& os, const SingleVectorPileStep& s) {
    if (s.is_stock_move()) {
        os << "STOCK_MOVE";
        if (!s.card_code())
            return os;
    }
    os << "[" << Card(s.card_code()) << ":"  << s.pile_from() + 1 << "->" << s.pile_to() + 1 << "]" << s.weight() << ";";
    // os << "{" << s.turned_up() << ", " << s.card_pos() << ", " << s.new_pos() << "}";
    // os << "(" << std::hex << s.data << std::dec << ")";
    return os;
}
