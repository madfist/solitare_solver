#ifndef SOLITARE_SOLVER_DECK_HEADER
#define SOLITARE_SOLVER_DECK_HEADER

#include <vector>
#include <list>
#include <algorithm>

#include "Card.hpp"

typedef std::list<Card> Pile;

std::ostream& operator<<(std::ostream& os, const Pile& p);
std::istream& operator>>(std::istream& is, Pile& p);

///Standard 52 card deck
class Deck {
public:
    Deck(bool ut = true);

    void shuffle();

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

    template<class Func>
    Func deal(Func f) const {
        return std::for_each(deck.rbegin(), deck.rend(), f);
    }

    friend std::ostream& operator<<(std::ostream& os, const Deck& d);
    friend std::istream& operator>>(std::istream& is, Deck& d);
private:
    std::vector<Card> deck;
    std::vector<Card>::reverse_iterator cards_left;
};

#endif