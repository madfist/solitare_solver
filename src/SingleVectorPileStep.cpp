#include <ostream>
#include <regex>

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
    return weight() > s.weight() || (weight() == s.weight() && pile_from() < s.pile_from());
}

void SingleVectorPileStep::parse(const std::string& s) {
    // std::cout << s << " ~ ";
    std::regex rgx("(STOCK_MOVE)?\\[([DCHS][0-9TJQKA][_^]):(\\d+):(\\d+)->(\\d+):(\\d+)\\]([_^])(\\d+);");
    std::smatch result;
    std::regex_search(s, result, rgx);
    // for (int i = 1; i < result.size(); ++i) {
    //     std::cout << result[i] << " X ";
    // }
    // std::cout << '\n';
    set(STOCK_STEP, !result[1].str().empty());
    set(CARD_CODE, Card(result[2]).get());
    set(PILE_FROM, std::stoi(result[3]) - 1);
    set(CARD_POS, std::stoi(result[4]));
    set(PILE_TO, std::stoi(result[5]) - 1);
    set(NEW_POS, std::stoi(result[6]));
    set(TURNED_UP, (result[7].str()[0] == '^'));
    set(WEIGHT, std::stoi(result[8]));
}

std::ostream& operator<<(std::ostream& os, const SingleVectorPileStep& s) {
    if (s.is_stock_move()) {
        os << "STOCK_MOVE";
        if (!s.card_code())
            return os;
    }
    os << "[" << Card(s.card_code()) << ":"  << s.pile_from() + 1 << ":" << s.card_pos() << "->" << s.pile_to() + 1 << ":" << s.new_pos() << "]" << (s.turned_up() ? "^" : "_") << s.weight() << ";";
    return os;
}

std::istream& operator>>(std::istream& is, SingleVectorPileStep& s) {
    std::string step_str;
    is >> step_str;
    // std::cout << "ss" << step_str << '\n';
    s.parse(step_str);

    return is;
}
