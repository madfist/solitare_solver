/**
 * Card representation
 * @file Card.hpp
 * @author Adam Koleszar
 */
#ifndef SOLITARE_SOLVER_CARD_HEADER
#define SOLITARE_SOLVER_CARD_HEADER

#include <string>

/**
 * @brief Normal french cards A-2-10-J-Q-K, spade, club, heart, diamond
 *
 * Represented by an 8bit integer [7 SEPARATOR][6 UPTURNED][4-5 SUITE][0-3 RANK]
 */
class Card {
public:
    Card();
    Card(unsigned char);
    Card(const std::string&);

    Card(const Card&);
    Card& operator=(const Card&);

    void set(unsigned char);
    void rank(unsigned char);
    void suite(unsigned char);
    bool upturned();
    void turnup(bool);

    bool operator==(const Card&) const;
    bool operator!=(const Card&) const;

    friend std::ostream& operator<<(std::ostream& os, const Card& c); ///< Stream output operator @param os stream @param c Card @return stream
    friend std::istream& operator>>(std::istream& is, Card& c);       ///< Stream input operator @param is stream @param c Card @return stream
private:
    void parse(const std::string&);
    std::string print() const;

    static unsigned char search_card(char);
    static char card_strings[];

    unsigned char card;
};

#endif