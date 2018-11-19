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

typedef uint8_t CardCode;

std::ostream& operator<<(std::ostream&, const CardCode&);

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
    static const CardCode CARD_SEPARATOR;
    Card();
    Card(CardCode);
    Card(const std::string&);

    ~Card() override;

    Card& operator=(const CardCode&);

    CardCode rank() const;
    Card& rank(CardCode);
    CardCode suite() const;
    Card& suite(CardCode);
    Card& turnup(bool ut = true);

    static CardCode& turnup(CardCode&, bool ut = true);

    explicit operator bool() const; ///< checks if upturned
    explicit operator CardCode() const;

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