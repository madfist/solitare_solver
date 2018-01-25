#include <sstream>

#include "gtest/gtest.h"
#include "Card.hpp"

static const unsigned char SA_ = 0x00;
static const unsigned char S7_ = 0x06;
static const unsigned char SK_ = 0x0C;
static const unsigned char CA_ = 0x10;
static const unsigned char C7_ = 0x16;
static const unsigned char SAU = 0x40;

TEST(test_card, print) {
    std::stringstream ss;
    ss << Card(SA_) << Card(SK_) << Card(CA_) << Card(SAU);
    EXPECT_EQ(ss.str(), "SA_SK_CA_SA^");
}

TEST(test_card, equals) {
    EXPECT_EQ(Card(), Card(SA_));
    EXPECT_EQ(Card(CA_), Card(CA_));
    EXPECT_EQ(Card(SK_), Card("SK_"));
    EXPECT_EQ(Card(SAU), Card("SA^"));
    EXPECT_TRUE(Card(SAU) != Card(CA_));
}

TEST(test_card, parse) {
    std::istringstream in("SA^");
    Card card(0);
    in >> card;
    EXPECT_EQ(card, Card(SAU));
}

TEST(test_card, set) {
    Card card;
    card.rank(6);
    EXPECT_EQ(Card(S7_), card);
    card.suite(1);
    EXPECT_EQ(Card(C7_), card);
    card.set(0);
    EXPECT_EQ(Card(SA_), card);
    card.turnup(true);
    EXPECT_EQ(Card(SAU), card);
}

TEST(test_card, get) {
    Card card(C7_);
    EXPECT_EQ(card.rank(), 6);
    EXPECT_EQ(card.suite(), 1);
    EXPECT_EQ(card.upturned(), 0);
    EXPECT_EQ(Card().rank(), 0);
}

TEST(test_card, io) {
    std::istringstream in("CA^ HK_ D3_");
    Card card;
    in >> card;
    EXPECT_EQ(card, Card("CA^"));
    in >> card;
    EXPECT_EQ(card, Card("HK_"));
    in >> card;
    EXPECT_EQ(card, Card("D3_"));
    
    std::stringstream out;
    out << card;
    EXPECT_EQ(out.str(), "D3_");
}