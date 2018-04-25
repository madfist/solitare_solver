#ifndef SOLITARE_SOLVER_GAME_STATE_HEADER
#define SOLITARE_SOLVER_GAME_STATE_HEADER

#include <vector>
#include <ostream>

#include "Card.hpp"

class SingleVectorGameState {
public:
    SingleVectorGameState();
    SingleVectorGameState(unsigned state_size, unsigned piles);
    SingleVectorGameState(const SingleVectorGameState&);
    ~SingleVectorGameState();

    CardCode& operator[](std::size_t);
    const CardCode& operator[](std::size_t) const;

    bool operator==(const SingleVectorGameState&) const;
    explicit operator bool() const;

    std::size_t hash() const;
    void reset();
    bool pile_empty(unsigned) const;
    unsigned pile_size(unsigned) const;
    unsigned pile_bottom(unsigned) const;
    unsigned pile_top(unsigned) const;
    void move_cards_backward(unsigned, unsigned, unsigned, unsigned);
    void move_cards_forward(unsigned, unsigned, unsigned, unsigned);
    unsigned find_card(const CardCode&) const;

    friend std::ostream& operator<<(std::ostream&, const SingleVectorGameState&);
private:
    std::vector<CardCode> state;
    unsigned last_pile;
};

#endif