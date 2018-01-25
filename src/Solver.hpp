#ifndef SOLITARE_SOLVER_SOLVER_HEADER
#define SOLITARE_SOLVER_SOLVER_HEADER

#include <memory>
#include <forward_list>
#include <iostream>
#include <algorithm>

#include "Game.hpp"

template<class Step>
class Solution {
public:
    Solution() : steps() {}
    void add(const Step& s) {
        steps.push_front(s);
    }
    friend std::ostream& operator<<(std::ostream& os, const Solution<Step>& s) {
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
    std::forward_list<Step> steps;
};

template<class Step>
class Solver {
public:
    Solver(std::shared_ptr<Game<Step>> g) : game(g) {}

    Solution<Step> solve() {
        Solution<Step> solution;
        auto last_step = steps.begin();
        auto prev_step = last_step;
        while (!game->win()) {
            auto new_steps = game->valid_steps();
            if (!new_steps.empty()) {
                steps.splice(last_step, new_steps);
                game->do_step(*last_step);
                prev_step = last_step++;
            } else {
                game->undo_step(*prev_step);
                if (last_step != steps.end())
                    game->do_step(*last_step);
            }
        }
        return solution;
    }
private:
    Solver();
    Solver(const Solver&);
    Solver& operator=(const Solver&);

    std::shared_ptr<Game<Step>> game;
    std::list<Step> steps;
};

#endif