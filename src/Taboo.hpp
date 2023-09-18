#ifndef TABOO_HEADER
#define TABOO_HEADER

#include <unordered_set>
#include <mutex>
#include <shared_mutex>

#include "Game.hpp"
#include "SingleVectorGameState.hpp"

/**
 * Stores unsuccesful game state hashes
 * @tparam Step type of game step
 */
template<class Step, class State = SingleVectorGameState>
class Taboo {
public:
    virtual ~Taboo() {}
    virtual void add(const State& elem) {
        taboo.insert(elem);
    }
    virtual bool check(const State& elem) const {
        return taboo.find(elem) != taboo.end();
    }
    virtual std::size_t size() const {
        return taboo.size();
    }
private:
    std::unordered_set<State, StateHash<State>> taboo;
};

template<class Step, class State = SingleVectorGameState>
class ConcurrentTaboo : public Taboo<Step, State> {
public:
    ~ConcurrentTaboo() override {}
    void add(const State& elem) override {
        std::unique_lock lock(mx);
        Taboo<Step>::add(elem);
    }
    bool check(const State& elem) const override {
        std::shared_lock lock(mx);
        return Taboo<Step>::check(elem);
    }
    std::size_t size() const override {
        std::shared_lock lock(mx);
        return Taboo<Step>::size();
    }
private:
    mutable std::shared_mutex mx;
};

#endif
