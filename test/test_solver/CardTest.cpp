#include <sstream>

#include "gtest/gtest.h"
#include "Card.hpp"

static const CardCode SA_ = 0x00;
static const CardCode S7_ = 0x06;
static const CardCode SK_ = 0x0C;
static const CardCode CA_ = 0x10;
static const CardCode C7_ = 0x16;
static const CardCode SAU = 0x40;

TEST(test_card, print) {
    std::stringstream ss;
    ss << Card(SA_) << Card(SK_) << Card(CA_) << Card(SAU);
    EXPECT_EQ(ss.str(), "SA_SK_CA_SA^");
}

TEST(test_card, equals) {
    EXPECT_EQ(Card(), Card(Card::CARD_SEPARATOR));
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
    Card card(SA_);
    EXPECT_EQ(Card(S7_), card.rank(6));
    EXPECT_EQ(Card(C7_), card.suite(1));
    EXPECT_EQ(Card(SA_), card.set(0));
    EXPECT_EQ(Card(SAU), card.turnup(true));
    CardCode cc = SAU;
    EXPECT_EQ(SA_, Card::turnup(cc, false));

    Card sep = Card();
    EXPECT_EQ(sep.get(), Card::CARD_SEPARATOR);
}

TEST(test_card, get) {
    Card card("C7_");
    EXPECT_EQ(card.get(), C7_);
    EXPECT_EQ(card.rank(), 6);
    EXPECT_EQ(card.suite(), 1);
    EXPECT_EQ(card.upturned(), false);
    EXPECT_EQ(Card::upturned(C7_), false);
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