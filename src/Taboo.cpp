#include "Taboo.hpp"

Taboo::~Taboo() {}

void Taboo::add(std::size_t elem) {
    taboo.insert(elem);
}

bool Taboo::check(std::size_t elem) const {
    return taboo.find(elem) != taboo.end();
}

std::size_t Taboo::size() const {
    return taboo.size();
}

ConcurrentTaboo::~ConcurrentTaboo() {}

void ConcurrentTaboo::add(std::size_t elem) {
    std::unique_lock<ting::shared_mutex> lock(mx);
    Taboo::add(elem);
}

bool ConcurrentTaboo::check(std::size_t elem) const {
    ting::shared_lock<ting::shared_mutex> lock(mx);
    return Taboo::check(elem);
}

std::size_t ConcurrentTaboo::size() const {
    ting::shared_lock<ting::shared_mutex> lock(mx);
    return Taboo::size();
}