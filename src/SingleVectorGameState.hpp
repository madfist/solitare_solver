/**
 * Single-vector game state
 * @file SingleVectorGameState.hpp
 * @author Adam Koleszar
 */
#ifndef SOLITARE_SOLVER_GAME_STATE_HEADER
#define SOLITARE_SOLVER_GAME_STATE_HEADER

#include <vector>
#include <ostream>
#include <functional>

#include "Card.hpp"
#include "SingleVectorPileStep.hpp"
#include "PrettyPrint.hpp"

using CardCodeFn = std::function<void(unsigned pos, const CardCode& card)>; ///< Card handling function
using CardFn = std::function<void(unsigned pos, const Card& card)>;         ///< Card handling function

/// Game state represented as a vector of ::CardCode
class SingleVectorGameState {
public:
    SingleVectorGameState(); ///< Create empty state
    /**
     * @brief Create empty state
     * @param state_size number of cards
     * @param piles number of piles
     */
    SingleVectorGameState(unsigned state_size, unsigned piles);
    SingleVectorGameState(const SingleVectorGameState&);
    ~SingleVectorGameState();

    CardCode& operator[](std::size_t pos);               ///< Get card at position @param pos card position
    const CardCode& operator[](std::size_t pos) const;   ///< Get card at position @param pos card position

    bool operator==(const SingleVectorGameState&) const; ///< Compare states
    explicit operator bool() const;                      ///< Check state is not empty and valid

    /// Helper class for piles
    class Pile {
    public:
        /**
         * @brief Get a pile from game
         * @param ref game state
         * @param pile_no pile number
         */
        Pile(const SingleVectorGameState& ref, unsigned pile_no);

        CardCode bottom(unsigned offset = 0) const; ///< Get card from the bottom of the pile @param offset offset from bottom
        CardCode top(unsigned offset = 0) const;    ///< Get card from the top of the pile @param offset offset from top
        unsigned size() const;                      ///< Get pile size
        bool empty() const;                         ///< Check if pile is empty

        void top_to_bottom(CardCodeFn) const;       ///< Run card function from top to bottom of the pile 
        void top_to_bottom(CardFn) const;           ///< Run card function from top to bottom of the pile 
    private:
        unsigned pile_no;
        const SingleVectorGameState& ref;
    };

    const Pile operator()(unsigned p) const; ///< Get pile @param p pile number

    std::size_t hash() const;
    void reset();                            ///< Reset game state to empty
    void reset(unsigned size, unsigned p);   ///< Reset and resize game state @param size new size @param p number of piles

    bool pile_empty(unsigned p) const;       ///< Check if pile is empty
    unsigned pile_size(unsigned p) const;    ///< Get pile size
    CardCode pile_bottom(unsigned p) const;  ///< Get card from the bottom of the pile
    CardCode pile_top(unsigned p) const;     ///< Get card from the top of the pile

    void do_move_and_upturn(const SingleVectorPileStep& s);
    void undo_move_and_upturn(const SingleVectorPileStep& s);

    void move_cards_backward(unsigned, unsigned, unsigned, unsigned);
    void move_cards_forward(unsigned, unsigned, unsigned, unsigned);
    void move_single_card_backward(unsigned, unsigned, unsigned, unsigned);
    void move_single_card_forward(unsigned, unsigned, unsigned, unsigned);

    unsigned find_card(const CardCode&) const;
    unsigned first_card_pos() const;

    friend std::ostream& operator<<(std::ostream&, const SingleVectorGameState&);
    template<class Output>
    friend Output& operator<<(PrettyPrintWrapper<Output>, const SingleVectorGameState&);
private:
    std::vector<CardCode> state;
    unsigned last_pile;
};

template<class Output>
Output& operator<<(PrettyPrintWrapper<Output> ppw, const SingleVectorGameState& gs) {
    unsigned i = gs.last_pile;
    for (unsigned p = 0; p <= gs.last_pile; ++p) {
        ppw.output << p+1 << ":";
        for (; i < gs.state.size() && i < gs[p]; ++i) {
            ppw.output << Card(gs[i]);
            if (i < gs[p] - 1)
                ppw.output << ' ';
        }
        if (p < gs.last_pile)
            ppw.output << '\n';
    }
    return ppw.output;
}

#endif