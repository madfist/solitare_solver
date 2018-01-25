#include "Rules.hpp"

static const CardCode SPADE   = 0;
static const CardCode CLUB    = 1;
static const CardCode HEART   = 2;
static const CardCode DIAMOND = 3;

Rules::Rules() : suite_order(SAME), rank_order(ACE_KING_DISABLED) {}

Rules::Rules(SuiteOrder so, RankOrder ro) : suite_order(so), rank_order(ro) {}

bool Rules::is_before(const Card& c1, const Card& c2) const {
    bool result = (c1.upturned() == c2.upturned());
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

bool Rules::is_alternate_suite(const CardCode& s1, const CardCode& s2) const {
    if (s1 == SPADE || s1 == CLUB) {
        return (s2 == HEART || s2 == DIAMOND);
    }
    return (s2 == SPADE || s2 == CLUB);
}