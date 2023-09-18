#include <iostream>
#include <sstream>
#include <iterator>

#include "KlondikeGame.hpp"
#include "RuledCard.hpp"
#include "Trace.hpp"

static const unsigned STATE_SIZE = Deck::DECK_SIZE + 11; // 52 cards + 11 index values for piles_start
static const unsigned GAME_PILES = 7;
static const unsigned FOUNDATION_PILES = 4;
static const unsigned STOCK_PILE = 11;
static const unsigned ALL_PILES = 12;

static const CardCode KING = 12;
static const CardCode ACE = 0;

KlondikeGame::KlondikeGame()
        : state_()
        , pile_rules(Rules::ALTERNATE, Rules::ACE_KING_DISABLED)
        , foundation_rules(Rules::SAME, Rules::ACE_KING_DISABLED)
        , check_further(true) {}

KlondikeGame::KlondikeGame(Deck& deck, bool shuffle /*false*/)
        : state_(STATE_SIZE, ALL_PILES)
        , pile_rules(Rules::ALTERNATE, Rules::ACE_KING_DISABLED)
        , foundation_rules(Rules::SAME, Rules::ACE_KING_DISABLED)
        , check_further(true) {
    if (shuffle)
        deck.shuffle();
    unsigned i = state_.first_card_pos(), pile = 0, pile_size = 1, dealt = 0;
    state_[STOCK_PILE] = STATE_SIZE;
    deck.deal([&] (const Card& c) {
        Card card(c);
        if (pile < GAME_PILES) {
            if (i < state_.first_card_pos() + dealt + pile_size - 1) {
                card.turnup(false);
            }
            state_[i++] = card.get();
            if (i == state_.first_card_pos() + dealt + pile_size) {
                state_[pile++] = i;
                dealt += pile_size++;
            }
        } else {
            if (pile < state_.first_card_pos()) {
                state_[pile++] = dealt + state_.first_card_pos();
            }
            // STOCK
            state_[i++] = card.get();
        }
    });
}

bool KlondikeGame::operator==(const Game& g) const {
    try {
        const KlondikeGame& kg = dynamic_cast<const KlondikeGame&>(g);
        return state_ == kg.state_;
    } catch (const std::bad_cast& e) {
        return false;
    }
}

KlondikeGame::operator bool() const {
    return (state_) ? true : false;
}

std::size_t KlondikeGame::hash() const {
    return state_.hash();
}

void KlondikeGame::do_step(const KlondikeStep& s) {
    Trace(TraceComponent::GAME) << "DO " << s;
    if (s.is_stock_move()) {
        state_.move_single_card_forward(s.pile_from(), s.pile_to(), s.card_pos(), s.new_pos());
    } else {
        state_.do_move_and_upturn(s);
    }
    Trace(TraceComponent::GAME) << *this;
}

void KlondikeGame::undo_step(const KlondikeStep& s) {
    Trace(TraceComponent::GAME) << "UNDO " << s;
    if (s.is_stock_move()) {
        // struct PrettyPrint pretty;
        // std::cout << pretty << state_ << std::endl;
        state_.move_single_card_backward(s.pile_to(), s.pile_from(), s.new_pos(), s.card_pos());
        // std::cout << pretty << state_ << std::endl;
    } else {
        state_.undo_move_and_upturn(s);
    }
    Trace(TraceComponent::GAME) << *this;
}

std::vector<KlondikeStep> KlondikeGame::valid_steps() const {
    std::vector<KlondikeStep> steps;

    for (pile from = 0; from < ALL_PILES; ++from) {
        for (pile to = 0; to < GAME_PILES + FOUNDATION_PILES; ++to) {
            if (from == to || state_(from).empty() || (from >= GAME_PILES && from != STOCK_PILE && to >= GAME_PILES))
                continue;
            // state_(from).top_to_bottom([&](unsigned i, const CardCode& cc) {
            for (unsigned i = state_.pile_top(from); i >= state_.pile_bottom(from); --i) {
                CardCode cc = state_[i];
                unsigned weight = 3;
                bool step_found = false;
                if (!Card(cc) || (to >= GAME_PILES && i < state_.pile_top(from)))
                    break;//return;
                // king/ace step
                if (state_(to).empty()) {
                    if (Card(cc).rank() == KING && to < GAME_PILES) {
                        // don't move kings between empty piles
                        if (cc == state_(from).bottom())
                            continue;//return;
                        weight = 2;
                        step_found = true;
                        // std::cout << "| king step " << cc << ":" << from << "->" << to << std::endl;
                    }
                    else if (Card(cc).rank() == ACE && to >= GAME_PILES) {
                        // don't move aces to more than one empty pile
                        if (to - 1 >= GAME_PILES && state_(to - 1).empty())
                            break;//return;
                        weight = 3;
                        step_found = true;
                        // std::cout << "| ace step " << cc << ":" << from << "->" << to << std::endl;
                    }
                }
                // normal step
                else {
                    if (from >= GAME_PILES && from != STOCK_PILE && to < GAME_PILES)// if (Card(cc).rank() == ACE)
                        continue;
                    RuledCard rc(cc, (to < GAME_PILES) ? pile_rules : foundation_rules);
                    step_found = (to < GAME_PILES) ? (rc < state_(to).top()) : (rc > state_(to).top());
                    // if (step_found) std::cout << "| normal step " << cc << ":" << from << "->" << to << std::endl;
                }
                if (step_found) {
                    if (i > state_.pile_bottom(from) && Card(state_[i-1]) && Card(state_[i-1]).rank() == Card(state_(to).top()).rank() && from != STOCK_PILE) {
                        // std::cout << "| invariant step " << cc << ":" << from << "->" << to << std::endl;
                        if (!check_next_step_for_circle(KlondikeStep(cc, from, to, i - state_.pile_bottom(from), state_(to).size(), 0, false))) {
                            // std::cout << "| invariant step " << cc << ":" << from+1 << "->" << to+1 << std::endl;
                            continue;
                        }
                        weight = 0;
                    }
                    if (from >= GAME_PILES)
                        weight = 0;
                    else if (to >= GAME_PILES) {
                        // std::cout << "| foundation step " << cc << ":" << from << "->" << to << std::endl;
                        weight = 2;
                    }
                    steps.emplace_back(cc, from, to, i - state_.pile_bottom(from), state_(to).size(), (from == STOCK_PILE) ? 2 : weight, (from == STOCK_PILE));
                    // std::cout << "\tstore: " << steps.back();
                    if (i > state_.pile_bottom(from) && !Card(state_[i-1]) && state_[i] == steps.back().card_code()) {
                        steps.back().turned_up(true);
                        // std::cout << steps.back();
                    }
                    // std::cout << std::endl;
                }
            }//);
        }
    }

    // std::cout << "klondike steps: " << steps << std::endl;
    std::sort(steps.begin(), steps.end());
    return steps;
}

bool KlondikeGame::win() const {
    // check for folded cards in the piles. If there isn't any the game is practically done.
    for (unsigned p = 0; p < GAME_PILES; ++p) {
        if (state_.pile_empty(p)) {
            continue;
        }
        for (unsigned i = state_.pile_bottom(p); i < state_.pile_top(p); ++i) {
            if (!Card(state_[i])) {
                return false;
            }
        }
    }
    return true;
}

bool KlondikeGame::sanity() const {
    // no method to decide from initial state_
    return true;
}

std::ostream& KlondikeGame::print(std::ostream& os) const {
    unsigned i = state_.first_card_pos();
    for (unsigned p = 0; p < GAME_PILES; ++p) {
        std::cout << " " << p+1 << ":";
        for (; i < state_[p]; ++i) {
            os << Card(state_[i]);
            if (i < state_[p] - 1)
                os << " ";
        }
        os << '\n';
    }
    for (unsigned p = GAME_PILES; p < GAME_PILES + FOUNDATION_PILES; ++p) {
        std::cout << "F" << p+1-GAME_PILES << ":";
        for (; i < state_[p]; ++i) {
            os << Card(state_[i]);
            if (i < state_[p] - 1)
                os << " ";
        }
        os << '\n';
    }
    os << " S:";
    for (; i<STATE_SIZE; ++i) {
        os << Card(state_[i]);
        if (i < STATE_SIZE - 1)
            os << " ";
    }
    return os;
}

std::istream& KlondikeGame::read(std::istream& is) {
    state_.reset(STATE_SIZE, ALL_PILES);
    unsigned i = state_.first_card_pos();
    for (unsigned p = 0; p <= GAME_PILES + FOUNDATION_PILES; ++p) {
        std::string line;
        std::getline(is, line);
        if (line.find(':') != std::string::npos)
            line = line.substr(2, line.length()-2);
        std::stringstream ss(line);

        if (ss.peek() == '\r')
            ss.get();
        if (ss.peek() == EOF)
            ss.get();
        while(!ss.eof() && ss.peek() != '\r') {
            Card c;
            ss >> c;
            state_[i++] = c.get();
        }
        if (p < GAME_PILES + FOUNDATION_PILES)
            state_[p] = i;
    }
    return is;
}

bool KlondikeGame::check_next_step_for_circle(const KlondikeStep& s) const {
    if (check_further) {
        KlondikeGame next_game(*this);
        next_game.check_further = false;
        // KlondikeStep next_step(cc, from, to, i - state_.pile_bottom(from), state_(to).size());
        next_game.do_step(s);
        // auto step = -next_game.valid_steps().front();
        // std::cout << s << " vs. " << step << std::endl;
        // if (s == step) {
        //     // std::cout << "INVARIANT STEP " << s << std::endl;
        //     return false;
        // }
        auto steps = next_game.valid_steps();
        // auto next_step = std::find(steps.begin(), steps.end(), -s);
        auto next_step = std::find_if(steps.begin(), steps.end(), [](auto ks) -> bool {
            return ks.weight() > 0;
        });
        return (next_step != steps.end());
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const KlondikeGame& g) {
    return g.print(os);
}

std::istream& operator>>(std::istream& is, KlondikeGame& g) {
    return g.read(is);
}
