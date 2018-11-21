#ifndef SOLITARE_SOLVER_SOLUTION_HEADER
#define SOLITARE_SOLVER_SOLUTION_HEADER

#include <algorithm>
#include <forward_list>
#include <iostream>

template<class Step>
class Solution {
public:
    Solution() : steps(), total_steps(0), taboo_hits(0) {}

    void add(const Step& s) {
        steps.push_front(s);
    }

    Solution& operator++() {
        ++total_steps;
        return *this;
    }

    Solution& operator--() { // this makes no sense I know, but it is fancy ;)
        ++taboo_hits;
        return *this;
    }

    Solution& operator+=(const Solution& sl) {
        total_steps += sl.total_steps;
        taboo_hits += sl.taboo_hits;
        max_level = std::max(max_level, sl.max_level);
        step_tree_size += sl.step_tree_size;
        taboo_tree_size += sl.taboo_tree_size;
    }

    explicit operator bool() const {
        return !steps.empty();
    }

    Solution& finish(unsigned ml, unsigned ss, unsigned ts) {
        max_level = ml;
        step_tree_size = ss;
        taboo_tree_size = ts;
        return *this;
    }

    bool insane() const {
        return total_steps == 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Solution<Step>& s) {
        os << "Total steps taken: " << s.total_steps << std::endl;
        os << "Taboo hits: " << s.taboo_hits << std::endl;
        os << "Maximum depth: " << s.max_level << std::endl;
        os << "Step tree size: " << s.step_tree_size << std::endl;
        os << "Taboo tree size: " << s.taboo_tree_size << std::endl;
        if (s.steps.empty()) {
            os << "No solution";
            return os;
        }
        std::for_each(s.steps.begin(), s.steps.end(), [&] (const Step& step) {
            os << step << " ";
        });
        return os;
    }

private:
    unsigned total_steps;
    unsigned taboo_hits;
    unsigned max_level;
    unsigned step_tree_size;
    unsigned taboo_tree_size;
    std::forward_list<Step> steps;
};

#endif
