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
    Card(unsigned char);
    Card(const std::string&);

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

// class Card {
// public:
//     enum Suite {
//         SPADE = 0, CLUB, HEART, DIAMOND
//     };

//     enum Rank {
//         CA = 0, C2, C3, C4, C5, C6, C7, C8, C9, C0, CJ, CQ, CK
//     };

//     Card();
//     Card(Suite, Rank);
//     Card(Suite, Rank, bool);

//     bool operator==(const Card& card) const;

//     unsigned char hash() const;

//     Suite suite() const;
//     void set_suite(Suite);
//     Rank rank() const;
//     void set_rank(Rank);
//     bool is_upturned() const;
//     void turn();
//     void turn(bool t);
// private:
//     Suite suite_;
//     Rank rank_;
//     bool upturned_;
// };

#endif