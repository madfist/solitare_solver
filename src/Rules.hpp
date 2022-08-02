/**
 * @brief General card game rules
 * @file Rules.hpp
 * @author Adam Koleszar
 */
#ifndef SOLITARE_SOLVER_RULES_HEADER
#define SOLITARE_SOLVER_RULES_HEADER

#include "Card.hpp"
#include <vector>

/**
 * Helper class to decide step validity
 */
class Rules {
public:
    /// Pile rule on available suites
    enum SuiteOrder {
        SAME,      ///< All card needs to be the same suite
        ALTERNATE, ///< Card can alternate between black and red suites
        ANY        ///< Any card can fit into the pile
    };
    /// Pile rule for continuing a pile after Ace or King
    enum RankOrder {
        ACE_KING_ENABLED, ///< A pile ending in Ace can be continued with a King
        ACE_KING_DISABLED ///< A pile ending in Ace cannot be continued with a King
    };

    Rules();
    Rules(SuiteOrder, RankOrder);

    bool is_before(const Card&, const Card&) const;         ///< Check if card order is valid
    bool is_before(const CardCode&, const CardCode&) const; ///< Check if card order is valid

    std::vector<Card> next(const Card&) const;         ///< Get next possible cards in the order
    std::vector<CardCode> next(const CardCode&) const; ///< Get next possible cards in the order

private:
    bool is_alternate_suite(const CardCode&, const CardCode&) const;
    SuiteOrder suite_order;
    RankOrder rank_order;
};

#endif
