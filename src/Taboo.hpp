#ifndef TABOO_HEADER
#define TABOO_HEADER

#include <unordered_set>
#include <shared_mutex>

/**
 * Stores unsuccesful game state hashes
 */
class Taboo {
public:
    virtual ~Taboo();
    virtual void add(std::size_t elem);
    virtual bool check(std::size_t elem) const;
    virtual std::size_t size() const;
private:
    std::unordered_set<std::size_t> taboo;
};

class ConcurrentTaboo : public Taboo {
public:
    ~ConcurrentTaboo() override;
    void add(std::size_t elem) override;
    bool check(std::size_t elem) const override;
    std::size_t size() const override;
private:
    mutable std::shared_mutex mx;
};

#endif
