/**
 * Deck of cards
 * @file Deck.hpp
 * @author Adam Koleszar
 */
#ifndef SOLITARE_SOLVER_DECK_HEADER
#define SOLITARE_SOLVER_DECK_HEADER

#include <vector>
#include <list>
#include <algorithm>

#include "Card.hpp"

typedef std::list<Card> Pile; ///< Card pile

std::ostream& operator<<(std::ostream& os, const Pile& p);
std::istream& operator>>(std::istream& is, Pile& p);

/// Standard 52 card deck
class Deck {
public:
    Deck(bool ut = true); ///< Create deck @param ut upturned state

    void shuffle();       ///< Shuffle cards

    /**
     * @brief Deal a pile with the same upturned state
     * @param[in] n number of cards to deal
     * @param[in] ut upturned
     * @return pile of cards
     */
    Pile deal(int n, bool ut);

    /**
     * @brief Deal a pile with downturned and upturned cards
     * @param[in] nd number of downturned cards
     * @param[in] nu number of upturned cards
     * @return pile of cards
     */
    Pile deal(int nd, int nu);

    /**
     * @brief Custom dealer function
     * @param f a void function that decides what to do with each card in the deck
     * @tparam CardConsumer void function that accepts a single Card
     */ 
    template<class CardConsumer>
    void deal(CardConsumer f) const {
        std::for_each(deck.rbegin(), deck.rend(), f);
    }

    static const unsigned DECK_SIZE;
    static const unsigned SUITE_SIZE;
    static const unsigned RANK_SIZE;

    friend std::ostream& operator<<(std::ostream& os, const Deck& d); ///< Stream output operator
    friend std::istream& operator>>(std::istream& is, Deck& d);       ///< Stream input operator
private:
    std::vector<Card> deck;
    std::vector<Card>::reverse_iterator cards_left;
};

#endif