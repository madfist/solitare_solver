#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Card.hpp"
#include "Deck.hpp"
#include "Game.hpp"

TEST(deck_test, deal) {
    Deck deck;
    Pile pile1 = deck.deal(1, true);
    EXPECT_EQ(Card("DK^"), pile1.front());
    Pile pile2 = deck.deal(5, false);
    EXPECT_EQ(5, pile2.size());
    EXPECT_EQ(Card("D8_"), pile2.front());
    EXPECT_EQ(Card("DQ_"), pile2.back());
    Pile pile3 = deck.deal(1, 1);
    EXPECT_EQ(2, pile3.size());
    EXPECT_EQ(Card("D6^"), pile3.front());
    EXPECT_EQ(Card("D7_"), pile3.back());
}

TEST(deck_test, deal_f) {
    Deck deck;
    GameState state;
    int i = 0;
    deck.deal([&] (const Card& c) {
        state.push_back(c.get());
    });
    EXPECT_EQ(Card("DK^"), Card(state[0]));
}

TEST(pile_test, io) {
    Pile pile;
    std::stringstream in("CA^ HK_ D3_");
    in >> pile;
    EXPECT_THAT(pile, testing::ElementsAre(Card("D3_"), Card("HK_"), Card("CA^")));
    std::stringstream out;
    out << pile;
    EXPECT_EQ(out.str(), "CA^ HK_ D3_");
}