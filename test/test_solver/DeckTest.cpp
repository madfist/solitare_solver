#include "gtest/gtest.h"
#include "Card.hpp"
#include "Deck.hpp"

TEST(deck_test, deal) {
    Deck deck;
    Pile pile1 = deck.deal(1, true);
    EXPECT_EQ(Card("SA^"), pile1.front());
    Pile pile2 = deck.deal(5, false);
    EXPECT_EQ(5, pile2.size());
    EXPECT_EQ(Card("S2_"), pile2.front());
    EXPECT_EQ(Card("S6_"), pile2.back());
    Pile pile3 = deck.deal(1, 1);
    EXPECT_EQ(2, pile3.size());
    EXPECT_EQ(Card("S7_"), pile3.front());
    EXPECT_EQ(Card("S8^"), pile3.back());
}