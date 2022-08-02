#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Card.hpp"
#include "Rules.hpp"

TEST(rules_test, is_before) {
    Rules same_rules;
    EXPECT_TRUE(same_rules.is_before(Card("C2_"), Card("C3_")));
    EXPECT_TRUE(!same_rules.is_before(Card("C2_"), Card("C3^")));
    EXPECT_TRUE(!same_rules.is_before(Card("C2_"), Card("S3_")));
    EXPECT_TRUE(!same_rules.is_before(Card("C3_"), Card("C2_")));
    EXPECT_TRUE(!same_rules.is_before(Card("CK_"), Card("CA_")));

    Rules alternate_rules(Rules::ALTERNATE, Rules::ACE_KING_ENABLED);
    EXPECT_TRUE(alternate_rules.is_before(Card("C2_"), Card("D3_")));
    EXPECT_TRUE(alternate_rules.is_before(Card("C2_"), Card("H3_")));
    EXPECT_TRUE(alternate_rules.is_before(Card("DK_"), Card("CA_")));
    EXPECT_TRUE(!alternate_rules.is_before(Card("C2_"), Card("C3_")));

    Rules any_rules(Rules::ANY, Rules::ACE_KING_DISABLED);
    EXPECT_TRUE(any_rules.is_before(Card("C2_"), Card("C3_")));
    EXPECT_TRUE(any_rules.is_before(Card("C2_"), Card("D3_")));
    EXPECT_TRUE(any_rules.is_before(Card("C2_"), Card("H3_")));
    EXPECT_TRUE(any_rules.is_before(Card("C2_"), Card("S3_")));
    EXPECT_TRUE(!any_rules.is_before(Card("CK_"), Card("CA_")));
}

TEST(rules_test, next) {
    Rules same_rules;
    EXPECT_THAT(same_rules.next(Card("C2_")), testing::ElementsAre(Card("C3_")));
    EXPECT_THAT(same_rules.next(Card("CK_")), testing::ElementsAre(Card("CK_")));

    Rules same_ace_king_rules(Rules::SAME, Rules::ACE_KING_ENABLED);
    EXPECT_THAT(same_ace_king_rules.next(Card("C2_")), testing::ElementsAre(Card("C3_")));
    EXPECT_THAT(same_ace_king_rules.next(Card("CK_")), testing::ElementsAre(Card("CA_")));

    Rules alternate_rules(Rules::ALTERNATE, Rules::ACE_KING_DISABLED);
    EXPECT_THAT(alternate_rules.next(Card("C2_")), testing::ElementsAre(Card("H3_"), Card("D3_")));
    EXPECT_THAT(alternate_rules.next(Card("CK_")), testing::ElementsAre(Card("HK_"), Card("DK_")));
    EXPECT_THAT(alternate_rules.next(Card("H2_")), testing::ElementsAre(Card("S3_"), Card("C3_")));
    EXPECT_THAT(alternate_rules.next(Card("HK_")), testing::ElementsAre(Card("SK_"), Card("CK_")));

    Rules any_rules(Rules::ANY, Rules::ACE_KING_DISABLED);
    EXPECT_THAT(any_rules.next(Card("C2_")), testing::ElementsAre(Card("H3_"), Card("D3_"), Card("S3_"), Card("C3_")));
    EXPECT_THAT(any_rules.next(Card("CK_")), testing::ElementsAre(Card("HK_"), Card("DK_"), Card("SK_"), Card("CK_")));
}
