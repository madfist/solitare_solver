/**
 * Card representation
 * @file Card.hpp
 * @author Adam Koleszar
 */
#ifndef SOLITARE_SOLVER_CARD_HEADER
#define SOLITARE_SOLVER_CARD_HEADER

#include <array>
#include <string>

#include "VersatileMask.hpp"

typedef uint8_t CardCode; ///< A byte to represent a card

std::ostream& operator<<(std::ostream&, const CardCode& cc); ///< Stream output operator

/// Enum for a card bitmask
enum CardMaskElement {
    RANK = 0, SUITE, UPTURNED, SEPARATOR
};

/**
 * @brief Normal french cards A-2-10-J-Q-K, spade, club, heart, diamond
 *
 * Represented by an 8bit integer [7 SEPARATOR][6 UPTURNED][4-5 SUITE][0-3 RANK]
 */
class Card : public VersatileMask<uint8_t, CardMaskElement> {
public:
    static const CardCode CARD_SEPARATOR; ///< Empty card (used for separating piles)
    Card();                               ///< Create empty card
    Card(CardCode);                       ///< Create from ::CardCode
    /**
     * @brief Create card from string
     * @details Parsing rules
     * * `#` separator
     * * `2-10, A, J, Q, K` ranks
     * * `S, C, H, D` suites
     * * `^, _` upturned state
     */
    Card(const std::string&);

    ~Card() override;                     ///< Destructor

    Card& operator=(const CardCode&);

    CardCode rank() const;                ///< Get rank (A-2-10-J-Q-K)
    Card& rank(CardCode);                 ///< Set rank
    CardCode suite() const;               ///< Get suite (spade, club, heart, diamond)
    Card& suite(CardCode);                ///< Set suite
    Card& turnup(bool ut = true);         ///< Change upturned state

    static CardCode& turnup(CardCode&, bool ut = true);          ///< Change upturned state

    explicit operator bool() const;                              ///< Check upturned state
    explicit operator CardCode() const;                          ///< Convert to ::CardCode

    friend std::ostream& operator<<(std::ostream&, const Card&); ///< Stream output operator @param os stream @param c Card @return stream
    friend std::istream& operator>>(std::istream&, Card&);       ///< Stream input operator @param is stream @param c Card @return stream
private:
    void parse(const std::string&);
    std::string print() const;

    uint8_t mask(CardMaskElement) const override;
    uint8_t shift(CardMaskElement) const override;

    static CardCode search_card(char);
    static char card_strings[];
    static const std::array<uint8_t, 4> MASKS;
    static const std::array<uint8_t, 4> SHIFTS;
};

#endif
