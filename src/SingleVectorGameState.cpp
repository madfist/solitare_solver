#include "SingleVectorGameState.hpp"
#include "crc.hpp"
#include <algorithm>
#include <numeric>

SingleVectorGameState::SingleVectorGameState() : state(), last_pile(0) {}

SingleVectorGameState::SingleVectorGameState(unsigned state_size, unsigned piles) : state(state_size), last_pile(piles - 1) {}

SingleVectorGameState::SingleVectorGameState(const SingleVectorGameState& s) : state(s.state), last_pile(s.last_pile) {}

SingleVectorGameState::~SingleVectorGameState() {}

CardCode& SingleVectorGameState::operator[](std::size_t pos) {
    return state[pos];
}

const CardCode& SingleVectorGameState::operator[](std::size_t pos) const {
    return state[pos];
}

bool SingleVectorGameState::operator==(const SingleVectorGameState& s) const {
    return std::equal(state.begin(), state.end(), s.state.begin());
}

SingleVectorGameState::operator bool() const {
    return !state.empty() && std::accumulate(state.begin(), state.end(), 0);
}

std::size_t SingleVectorGameState::hash() const {
    return crc(0, state.data(), state.size());
}

void SingleVectorGameState::reset() {
    std::size_t s = state.size();
    state.clear();
    state.resize(s);
}

void SingleVectorGameState::reset(unsigned size, unsigned p) {
    state.clear();
    state.resize(size);
    last_pile = p - 1;
}

bool SingleVectorGameState::pile_empty(unsigned p) const {
    if (p == 0)
        return (state[p] == last_pile);
    if (p == last_pile)
        return (state[p-1] == state.size());
    return state[p] == state[p-1];
}

unsigned SingleVectorGameState::pile_size(unsigned p) const {
    return pile_top(p) - pile_bottom(p) + 1;
}

unsigned SingleVectorGameState::pile_bottom(unsigned p) const {
    return (p == 0) ? last_pile : state[p-1];
}

unsigned SingleVectorGameState::pile_top(unsigned p) const {
    return (p == last_pile) ? state.size()-1 : state[p]-1;
}

const SingleVectorGameState::Pile SingleVectorGameState::operator()(unsigned p) const {
    return Pile(*this, p);
}

SingleVectorGameState::Pile::Pile(const SingleVectorGameState& s, unsigned p) : ref(s), pile_no(p) {}

CardCode SingleVectorGameState::Pile::bottom(unsigned offset) const {
    return ref[ref.pile_bottom(pile_no) + offset];
}

CardCode SingleVectorGameState::Pile::top(unsigned offset) const {
    return ref[ref.pile_top(pile_no) - offset];
}

unsigned SingleVectorGameState::Pile::size() const {
    return ref.pile_size(pile_no);
}

bool SingleVectorGameState::Pile::empty() const {
    return ref.pile_empty(pile_no);
}

/// Move cards towards the back of the vector
void SingleVectorGameState::move_cards_backward(unsigned from, unsigned to, unsigned card_pos, unsigned new_pos) {
    unsigned first, middle, last, diff;
    first = pile_bottom(from) + card_pos;
    middle = pile_top(from) + 1;
    last = pile_bottom(to) + new_pos;
    diff = middle - first;
    // std::cout << "mb f" << first << " m" << middle << " l" << last << " d" << diff << std::endl;

    auto st = state.begin();
    std::rotate(st + first, st + middle, st + last);

    for (unsigned p = from; p < to; ++p)
        state[p] -= diff;
}

/// Move cards towards the front of the vector
void SingleVectorGameState::move_cards_forward(unsigned from, unsigned to, unsigned card_pos, unsigned new_pos) {
    unsigned first, middle, last, diff;
    first = pile_bottom(to) + new_pos;
    middle = pile_bottom(from) + card_pos;
    last = pile_top(from) + 1;
    diff = last - middle;
    // std::cout << "mf f" << first << " m" << middle << " l" << last << " d" << diff << std::endl;

    auto st = state.begin();
    std::rotate(st + first, st + middle, st + last);

    for (unsigned p = to; p < from; ++p)
        state[p] += diff;
}

/// Move cards towards the back of the vector
void SingleVectorGameState::move_single_card_backward(unsigned from, unsigned to, unsigned card_pos, unsigned new_pos) {
    unsigned first, middle, last, diff;
    first = pile_bottom(from) + card_pos;
    middle = first + 1;
    last = pile_bottom(to) + new_pos;
    diff = middle - first;
    // std::cout << "mb f" << first << " m" << middle << " l" << last << " d" << diff << std::endl;

    auto st = state.begin();
    std::rotate(st + first, st + middle, st + last);

    for (unsigned p = from; p < to; ++p)
        state[p] -= diff;
}

/// Move cards towards the front of the vector
void SingleVectorGameState::move_single_card_forward(unsigned from, unsigned to, unsigned card_pos, unsigned new_pos) {
    unsigned first, middle, last, diff;
    first = pile_bottom(to) + new_pos;
    middle = first + 1;
    last = pile_top(from) + 1;
    diff = last - middle;
    // std::cout << "mf f" << first << " m" << middle << " l" << last << " d" << diff << std::endl;

    auto st = state.begin();
    std::rotate(st + first, st + middle, st + last);

    for (unsigned p = to; p < from; ++p)
        state[p] += diff;
}

void SingleVectorGameState::do_move_and_upturn(const SingleVectorPileStep& s) {
    if (s.pile_from() < s.pile_to()) {
        move_cards_backward(s.pile_from(), s.pile_to(), s.card_pos(), s.new_pos());
    } else {
        move_cards_forward(s.pile_from(), s.pile_to(), s.card_pos(), s.new_pos());
    }
    if (s.turned_up()) {
        Card::turnup(state[pile_bottom(s.pile_from()) + s.card_pos() - 1]);
    }
}

void SingleVectorGameState::undo_move_and_upturn(const SingleVectorPileStep& s) {
    if (s.pile_from() < s.pile_to()) {
        move_cards_forward(s.pile_to(), s.pile_from(), s.new_pos(), s.card_pos());
    } else {
        move_cards_backward(s.pile_to(), s.pile_from(), s.new_pos(), s.card_pos());
    }
    if (s.turned_up()) {
        Card::turnup(state[pile_bottom(s.pile_from()) + s.card_pos() - 1], false);
    }
}

unsigned SingleVectorGameState::find_card(const CardCode& cc) const {
    for (unsigned i = 0; i < state.size(); ++i) {
        if (state[i] == cc)
            return i;
    }
    return Card::CARD_SEPARATOR;
}

unsigned SingleVectorGameState::first_card_pos() const {
    return last_pile;
}

std::ostream& operator<<(std::ostream &os, const SingleVectorGameState &gs) {
    for (auto cc = gs.state.begin(); cc != gs.state.end(); ++cc) {
        os << '[' << *cc << ']';
    }
    return os;
}

PrettyPrintWrapper operator<<(std::ostream& os, PrettyPrint) {
    return PrettyPrintWrapper(os);
}

std::ostream& operator<<(PrettyPrintWrapper ppw, const SingleVectorGameState& gs) {
    unsigned i = gs.last_pile;
    for (unsigned p = 0; p <= gs.last_pile; ++p) {
        ppw.output_stream << p+1 << ":";
        for (; i < gs.state.size() && i < gs[p]; ++i) {
            ppw.output_stream << Card(gs[i]);
            if (i < gs[p] - 1)
                ppw.output_stream << ' ';
        }
        if (p < gs.last_pile)
            ppw.output_stream << '\n';
    }
    return ppw.output_stream;
}