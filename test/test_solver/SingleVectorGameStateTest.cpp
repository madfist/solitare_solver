#include <cstdarg>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Deck.hpp"

#define private public
#include "SingleVectorGameState.hpp"
#undef private

static struct PrettyPrint pretty_print;

std::shared_ptr<SingleVectorGameState> create_state_from_deck(Deck& deck) {
    std::shared_ptr<SingleVectorGameState> state(new SingleVectorGameState(Deck::DECK_SIZE + 3, 4));
    unsigned i = 3;
    deck.deal([&](const Card& c) {
        Card card(c);
        (*state)[i++] = card.get();
    });
    for (int j = 0; j < 3; ++j) {
        (*state)[j] = (j+1)*13 + 3;
    }
    return state;
}

std::shared_ptr<SingleVectorGameState> create_test_state(unsigned size, unsigned piles, ...) {
    std::shared_ptr<SingleVectorGameState> state(new SingleVectorGameState(size, piles));
    va_list args;
    va_start(args, piles);
    for (int i = 0; i < piles-1; ++i) {
        (*state)[i] = va_arg(args, unsigned);
    }
    va_end(args);
    return state;
}

TEST(svec_game_state, empty) {
    SingleVectorGameState state;
    EXPECT_TRUE(!state);
}

TEST(svec_game_state, reset) {
    Deck deck;
    auto state = create_state_from_deck(deck);
    EXPECT_TRUE(*state);

    state->reset();
    EXPECT_TRUE(!*state);
    EXPECT_THAT(state->state.size(), Deck::DECK_SIZE + 3);

    state->reset(3,1);
    EXPECT_THAT(state->state.size(), 3);
}

TEST(svec_game_state, pile_empty) {
    auto state = create_test_state(10, 3, 2, 2);

    EXPECT_TRUE(state->pile_empty(0));
    EXPECT_TRUE(state->pile_empty(1));

    (*state)[1] = 10;

    EXPECT_TRUE(state->pile_empty(2));
}

TEST(svec_game_state, pile_size) {
    auto state = create_test_state(10, 3, 5, 7);

    EXPECT_THAT(state->pile_size(0), 3);
    EXPECT_THAT(state->pile_size(1), 2);
    EXPECT_THAT(state->pile_size(2), 3);
}

TEST(svec_game_state, pile_bottom) {
    auto state = create_test_state(10, 3, 5, 7);

    EXPECT_THAT(state->pile_bottom(0), 2);
    EXPECT_THAT(state->pile_bottom(1), 5);
    EXPECT_THAT(state->pile_bottom(2), 7);
}

TEST(svec_game_state, pile_top) {
    auto state = create_test_state(10, 3, 5, 7);

    EXPECT_THAT(state->pile_top(0), 4);
    EXPECT_THAT(state->pile_top(1), 6);
    EXPECT_THAT(state->pile_top(2), 9);
}

TEST(svec_game_state, move_cards_backward) {
    Deck deck;
    auto state = create_state_from_deck(deck);
    // std::cout << pretty_print << *state << '\n' << *state << std::endl;
    state->move_cards_backward(0, 1, 1, 13);
    EXPECT_THAT((*state)[4], Card("HK^").get());
    EXPECT_THAT((*state)[17], Card("DQ^").get());
    // std::cout << pretty_print << *state << std::endl;
}

TEST(svec_game_state, move_cards_forward) {
    Deck deck;
    auto state = create_state_from_deck(deck);
    // std::cout << pretty_print << *state << '\n' << *state << std::endl;
    state->move_cards_forward(1, 0, 1, 13);
    EXPECT_THAT((*state)[4], Card("DQ^").get());
    EXPECT_THAT((*state)[29], Card("CK^").get());
    // std::cout << pretty_print << *state << std::endl;
}

TEST(svec_game_state, find_card) {
    Deck deck;
    auto state(create_state_from_deck(deck));
    EXPECT_THAT(state->find_card(Card("HA^").get()), 28);
}

TEST(svec_game_state, first_card_pos) {
    auto state(create_test_state(10, 3, 5, 7));
    EXPECT_THAT(state->first_card_pos(), 2);
}