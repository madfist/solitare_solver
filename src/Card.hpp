/**
 * Card representation
 * @file Card.hpp
 * @author Adam Koleszar
 */
#ifndef SOLITARE_SOLVER_CARD_HEADER
#define SOLITARE_SOLVER_CARD_HEADER

#include <string>

typedef unsigned char CardCode;

std::ostream& operator<<(std::ostream&, const CardCode&);

/**
 * @brief Normal french cards A-2-10-J-Q-K, spade, club, heart, diamond
 *
 * Represented by an 8bit integer [7 SEPARATOR][6 UPTURNED][4-5 SUITE][0-3 RANK]
 */
class Card {
public:
    Card();
    Card(CardCode);
    Card(const std::string&);

    Card(const Card&);
    Card& operator=(const Card&);

    CardCode get() const;
    void set(CardCode);
    CardCode rank() const;
    void rank(CardCode);
    CardCode suite() const;
    void suite(CardCode);
    bool upturned() const;
    void turnup(bool);

    static bool separator(CardCode);
    static Card card_separator();

    bool operator==(const Card&) const;
    bool operator!=(const Card&) const;

    friend std::ostream& operator<<(std::ostream&, const Card&); ///< Stream output operator @param os stream @param c Card @return stream
    friend std::istream& operator>>(std::istream&, Card&);       ///< Stream input operator @param is stream @param c Card @return stream
private:
    void parse(const std::string&);
    std::string print() const;

    static CardCode search_card(char);
    static char card_strings[];

    CardCode card;
};

#endif