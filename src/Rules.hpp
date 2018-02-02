#ifndef SOLITARE_SOLVER_RULES_HEADER
#define SOLITARE_SOLVER_RULES_HEADER

#include "Card.hpp"
#include <vector>

class Rules {
public:
    enum SuiteOrder {
        SAME, ALTERNATE, ANY
    };
    enum RankOrder {
        ACE_KING_ENABLED,
        ACE_KING_DISABLED
    };

    Rules();
    Rules(SuiteOrder, RankOrder);

    bool is_before(const Card&, const Card&) const;
    bool is_before(const CardCode&, const CardCode&) const;

    std::vector<Card> next(const Card&) const;
    std::vector<CardCode> next(const CardCode&) const;

private:
    bool is_alternate_suite(const CardCode&, const CardCode&) const;
    SuiteOrder suite_order;
    RankOrder rank_order;
};

#endif