#ifndef SOLITARE_SOLVER_SOLVER_HEADER
#define SOLITARE_SOLVER_SOLVER_HEADER

#include <forward_list>
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include "Game.hpp"

template<class Step>
class Solution {
public:
    Solution() : steps() {}
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
    Solution& finish(unsigned ml, unsigned ss, unsigned ts) {
        max_level = ml;
        step_tree_size = ss;
        taboo_tree_size = ts;
        return *this;
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

template<class Step>
class StepNode {
public:
    static int ROOT;
    static int END;
    StepNode(int r, int n, const Step& s) : root_(r), next_(n), step_(s) {}
    int root() { return root_; }
    int next() { return next_; }
    Step& step() { return step_; }
private:
    int root_;
    int next_;
    Step step_;
};

template<class Step> int StepNode<Step>::ROOT = -2;
template<class Step> int StepNode<Step>::END = -1;

template<class Step>
class Solver {
public:
    Solver(GamePtr<Step> g) : game(g), level(0), max_level(0) {}

    /**
     * @brief      Solve the game with BFS and taboo heap
     * The step tree is represented by a vector and we store "pointer" information inside as vector index
     * @return     solution
     */
    Solution<Step> solve() {
        Solution<Step> solution;
        int current_node_id = StepNode<Step>::ROOT;
        int next_node_id = 0;
        int i = 0;
        while (!game->win()) {
            ++solution;
            if (!is_taboo()) {
                next_node_id = next_node(current_node_id);
                add_taboo();
            } else {
                --solution;
            }

            //out of steps to take => undo until we find new steps
            if (next_node_id == nodes.size() || next_node_id == current_node_id) {
                // undo step
                game->undo_step(nodes[current_node_id].step());
                level--;

                // undo until we find a possible step to take
                next_node_id = undo_node(current_node_id);

                //if we got back to ROOT => return solution
                if (next_node_id == StepNode<Step>::ROOT)
                    return solution.finish(max_level, nodes.size(), 0);
            }

            game->do_step(nodes[next_node_id].step());
            ++level;
            current_node_id = next_node_id;
            max_level = std::max(level, max_level);
        }

        do {
            solution.add(nodes[next_node_id].step());
            next_node_id = nodes[next_node_id].root();
        } while (next_node_id != StepNode<Step>::ROOT);

        return solution.finish(max_level, nodes.size(), 0);
    }
private:
    Solver();
    Solver(const Solver&);
    Solver& operator=(const Solver&);

    /// Adds new node with new valid steps as leaves
    int next_node(int node_id) {
        auto steps = game->valid_steps();
        int last_node_id = nodes.size() + steps.size();
        int next_node_id = nodes.size() + 1;

        std::for_each(steps.begin(), steps.end(), [&] (const Step& s) {
            if (next_node_id == last_node_id)
                next_node_id = StepNode<Step>::END;
            nodes.emplace_back(node_id, next_node_id++, s);
        });

        return nodes.size() - steps.size();
    }

    /// Go back up nodes until we find a node to do
    int undo_node(int node_id) {
        // get previous step
        int node_root = nodes[node_id].root();

        // stop if we are already at ROOT
        if (node_root == StepNode<Step>::ROOT)
            return node_root;

        int next_node_id = nodes[node_root].next();

        // if we don't have other leaves go back up until we have usable leaves or reach ROOT
        while (next_node_id == StepNode<Step>::END) {
            // undo step
            game->undo_step(nodes[node_root].step());
            --level;

            // go back more
            node_root = nodes[node_root].root();

            // stop if we are already at ROOT
            if (node_root == StepNode<Step>::ROOT)
                return node_root;

            // set next node to try
            next_node_id = nodes[node_root].next();
        }
        game->undo_step(nodes[node_root].step());
        --level;
        return next_node_id;
    }

    bool is_taboo() const {
        auto t = taboo.find(game->state());
        return (t != taboo.end());
    }

    void add_taboo() {
        taboo.insert(game->state());
    }

    GamePtr<Step> game;
    std::vector<StepNode<Step>> nodes;
    std::unordered_set<GameState> taboo;
    unsigned level, max_level;
};

#endif