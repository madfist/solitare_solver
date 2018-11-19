#include "Rules.hpp"

static const CardCode SPADE   = 0;
static const CardCode CLUB    = 1;
static const CardCode HEART   = 2;
static const CardCode DIAMOND = 3;

static const CardCode KING = 12;

Rules::Rules() : suite_order(SAME), rank_order(ACE_KING_DISABLED) {}

Rules::Rules(SuiteOrder so, RankOrder ro) : suite_order(so), rank_order(ro) {}

bool Rules::is_before(const Card& c1, const Card& c2) const {
    bool result = ((bool)c1 == (bool)c2);
    switch (suite_order) {
        case SAME:
            result = result && c1.suite() == c2.suite(); break;
        case ALTERNATE:
            result = result && is_alternate_suite(c1.suite(), c2.suite()); break;
    }
    if (rank_order == ACE_KING_ENABLED) {
        return result && ((c1.rank()+1)%13 == c2.rank());
    }
    return result && (c1.rank()+1 == c2.rank());
}

bool Rules::is_before(const CardCode& cc1, const CardCode& cc2) const {
    return is_before(Card(cc1), Card(cc2));
}

std::vector<Card> Rules::next(const Card& c) const {
    std::vector<Card> cards;
    CardCode new_rank = (c.rank() == KING) ? c.rank() : c.rank()+1;
    if (rank_order == ACE_KING_ENABLED) {
        new_rank = (c.rank()+1)%13;
    }
    switch (suite_order) {
        case SAME:
            cards.push_back(Card(c).rank(new_rank)); break;
        case ALTERNATE:
            if (c.suite() == SPADE || c.suite() == CLUB) {
                cards.push_back(Card(c).rank(new_rank).suite(HEART));
                cards.push_back(Card(c).rank(new_rank).suite(DIAMOND));
            } else {
                cards.push_back(Card(c).rank(new_rank).suite(SPADE));
                cards.push_back(Card(c).rank(new_rank).suite(CLUB));
            }
            break;
        case ANY:
            cards.push_back(Card(c).rank(new_rank).suite(HEART));
            cards.push_back(Card(c).rank(new_rank).suite(DIAMOND));
            cards.push_back(Card(c).rank(new_rank).suite(SPADE));
            cards.push_back(Card(c).rank(new_rank).suite(CLUB));
            break;
    }
    return cards;
}

std::vector<CardCode> Rules::next(const CardCode& cc) const {
    auto cards = next(Card(cc));
    std::vector<CardCode> card_codes(cards.size());
    for (unsigned i = 0; i < cards.size(); ++i)
        card_codes[i] = cards[i].get();
    return card_codes;
}

bool Rules::is_alternate_suite(const CardCode& s1, const CardCode& s2) const {
    if (s1 == SPADE || s1 == CLUB) {
        return (s2 == HEART || s2 == DIAMOND);
    }
    return (s2 == SPADE || s2 == CLUB);
}