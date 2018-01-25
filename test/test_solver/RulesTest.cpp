#include "gtest/gtest.h"
#include "Card.hpp"
#include "Rules.hpp"

TEST(rules_test, is_before) {
    Rules rules1;
    EXPECT_TRUE(rules1.is_before(Card("C2_"), Card("C3_")));
    EXPECT_TRUE(!rules1.is_before(Card("C2_"), Card("C3^")));
    EXPECT_TRUE(!rules1.is_before(Card("C2_"), Card("S3_")));
    EXPECT_TRUE(!rules1.is_before(Card("C3_"), Card("C2_")));
    EXPECT_TRUE(!rules1.is_before(Card("CK_"), Card("CA_")));

    Rules rules2(Rules::ALTERNATE, Rules::ACE_KING_ENABLED);
    EXPECT_TRUE(rules2.is_before(Card("C2_"), Card("D3_")));
    EXPECT_TRUE(rules2.is_before(Card("C2_"), Card("H3_")));
    EXPECT_TRUE(rules2.is_before(Card("DK_"), Card("CA_")));
    EXPECT_TRUE(!rules2.is_before(Card("C2_"), Card("C3_")));

    Rules rules3(Rules::ANY, Rules::ACE_KING_DISABLED);
    EXPECT_TRUE(rules3.is_before(Card("C2_"), Card("C3_")));
    EXPECT_TRUE(rules3.is_before(Card("C2_"), Card("D3_")));
    EXPECT_TRUE(rules3.is_before(Card("C2_"), Card("H3_")));
    EXPECT_TRUE(rules3.is_before(Card("C2_"), Card("S3_")));
    EXPECT_TRUE(!rules3.is_before(Card("CK_"), Card("CA_")));
}