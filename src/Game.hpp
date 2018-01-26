#ifndef SOLITARE_SOLVER_GAME_HEADER
#define SOLITARE_SOLVER_GAME_HEADER

#include <memory>
#include <list>
#include <forward_list>
#include <algorithm>

#include "Card.hpp"
extern "C" uint32_t crc32c(uint32_t crc, const unsigned char *buf, size_t len);

typedef std::forward_list<CardCode> GameState;

std::ostream& operator<<(std::ostream&, const GameState&);
namespace std {
    template<> struct hash<GameState> {
        typedef GameState argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& gs) const noexcept {
            result_type hash = 0;
            std::for_each(gs.begin(), gs.end(), [&] (const CardCode& cc) {
                hash = crc32c(hash, &cc, 1);
            });
            return hash;
        }
    };
}

template<class Step>
class Game {
public:
    virtual ~Game() {}
    virtual bool operator==(const Game& g) const = 0;
    virtual bool empty() const = 0;
    virtual GameState state() const = 0;
    virtual void state(GameState&) = 0;
    virtual void do_step(Step& s) = 0;
    virtual void undo_step(Step& s) = 0;
    virtual std::list<Step> valid_steps() const = 0;
    virtual bool win() const = 0;
};

template<class Step>
using GamePtr = std::shared_ptr<Game<Step>>;

#endif