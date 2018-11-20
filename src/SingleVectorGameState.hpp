#ifndef SOLITARE_SOLVER_GAME_STATE_HEADER
#define SOLITARE_SOLVER_GAME_STATE_HEADER

#include <vector>
#include <ostream>
#include <functional>

#include "Card.hpp"
#include "SingleVectorPileStep.hpp"

class PrettyPrintWrapper {
public:
    PrettyPrintWrapper(std::ostream& os): output_stream(os) {}
    std::ostream& output_stream;
};

struct PrettyPrint {};

PrettyPrintWrapper operator<<(std::ostream& os, PrettyPrint);

using CardCodeFn = std::function<void(unsigned, const CardCode&)>;
using CardFn = std::function<void(unsigned, const Card&)>;

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

    class Pile {
    public:
        Pile(const SingleVectorGameState&, unsigned);

        CardCode bottom(unsigned offset = 0) const;
        CardCode top(unsigned offset = 0) const;
        unsigned size() const;
        bool empty() const;

        void top_to_bottom(CardCodeFn) const;
        void top_to_bottom(CardFn) const;
    private:
        unsigned pile_no;
        const SingleVectorGameState& ref;
    };

    const Pile operator()(unsigned) const;

    std::size_t hash() const;
    void reset();
    void reset(unsigned, unsigned);

    bool pile_empty(unsigned) const;
    unsigned pile_size(unsigned) const;
    unsigned pile_bottom(unsigned) const;
    unsigned pile_top(unsigned) const;

    void do_move_and_upturn(const SingleVectorPileStep& s);
    void undo_move_and_upturn(const SingleVectorPileStep& s);

    void move_cards_backward(unsigned, unsigned, unsigned, unsigned);
    void move_cards_forward(unsigned, unsigned, unsigned, unsigned);
    void move_single_card_backward(unsigned, unsigned, unsigned, unsigned);
    void move_single_card_forward(unsigned, unsigned, unsigned, unsigned);

    unsigned find_card(const CardCode&) const;
    unsigned first_card_pos() const;

    std::ostream& pretty_print(std::ostream&);
    friend std::ostream& operator<<(std::ostream&, const SingleVectorGameState&);
    friend std::ostream& operator<<(PrettyPrintWrapper, const SingleVectorGameState&);
private:
    std::vector<CardCode> state;
    unsigned last_pile;
};

#endif