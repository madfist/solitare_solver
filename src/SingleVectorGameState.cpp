#include "SingleVectorGameState.hpp"
#include "crc.hpp"
#include <algorithm>

SingleVectorGameState::SingleVectorGameState() : state(), last_pile(0) {}

SingleVectorGameState::SingleVectorGameState(unsigned state_size, unsigned piles) : state(state_size), last_pile(piles) {}

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
    return !state.empty();
}

std::size_t SingleVectorGameState::hash() const {
    return crc(0, state.data(), state.size());
}

void SingleVectorGameState::reset() {
    std::size_t s = state.size();
    state.clear();
    state.resize(s);
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

unsigned SingleVectorGameState::find_card(const CardCode& cc) const {
    for (unsigned i = 0; i < state.size(); ++i) {
        if (state[i] == cc)
            return i;
    }
}

std::ostream& operator<<(std::ostream &os, const SingleVectorGameState &gs) {
    for (auto cc = gs.state.begin(); cc != gs.state.end(); ++cc) {
        os << "[" << *cc << "]";
    }
    return os;
}