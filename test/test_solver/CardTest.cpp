#include <sstream>

#include "gtest/gtest.h"
#include "Card.hpp"

static const unsigned char SA_ = 0x00;
static const unsigned char SK_ = 0x0C;
static const unsigned char CA_ = 0x10;
static const unsigned char SAU = 0x40;

TEST(test_card, test_card_print) {
    std::stringstream ss;
    ss << Card(SA_) << Card(SK_) << Card(CA_) << Card(SAU);
    EXPECT_EQ(ss.str(), "SA_SK_CA_SA^");
}

TEST(test_card, test_card_equals) {
    EXPECT_EQ(Card(CA_), Card(CA_));
    EXPECT_EQ(Card(SK_), Card("SK_"));
    EXPECT_EQ(Card(SAU), Card("SA^"));
    EXPECT_TRUE(Card(SAU) != Card(CA_));
}

TEST(test_card, test_card_parse) {
    std::istringstream in("SA^");
    Card card(0);
    in >> card;
    EXPECT_EQ(card, Card(SAU));
}