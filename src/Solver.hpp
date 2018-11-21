#ifndef SOLITARE_SOLVER_SOLVER_HEADER
#define SOLITARE_SOLVER_SOLVER_HEADER

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include "Game.hpp"
#include "Solution.hpp"
#include "StepNode.hpp"
#include "Log.hpp"
#include "Taboo.hpp"

struct SolverProperties {
    bool filter;
};

template<class Step>
class Solver {
public:
    Solver(GamePtr<Step> g, std::shared_ptr<Taboo> t, SolverProperties p = {false}) : game(g), taboo(t), level(0), max_level(0), properties(p) {}

    /**
     * @brief      Solve the game with BFS and taboo heap
     * The step tree is represented by a vector and we store "pointer" information inside as vector index
     * @return     solution
     */
    Solution<Step> solve() {
        Solution<Step> solution;
        if (!game->sanity())
            return solution.finish(0, 0, 0);
        int current_node_id = StepNode<Step>::ROOT;
        int next_node_id = 0;
        int i = 0;
        while (!game->win()) {
            ++solution;
            if (!taboo->check(game->hash())) {
                next_node_id = next_node(current_node_id);
                taboo->add(game->hash());
            } else {
                --solution;
            }
            Log(Log::DEBUG) << "curr:" << current_node_id << "next:" << next_node_id;

            //out of steps to take => undo until we find new steps
            if (next_node_id == nodes.size() || next_node_id == current_node_id) {
                // undo step
                game->undo_step(nodes[current_node_id].step());
                --level;

                // undo until we find a possible step to take
                next_node_id = undo_node(current_node_id);

                //if we got back to ROOT => return solution
                if (next_node_id == StepNode<Step>::ROOT)
                    return solution.finish(max_level, nodes.size(), taboo->size());
            }

            game->do_step(nodes[next_node_id].step());
            Log(Log::DEBUG) << *game << std::endl;
            ++level;
            current_node_id = next_node_id;
            max_level = std::max(level, max_level);
        }

        do {
            solution.add(nodes[next_node_id].step());
            next_node_id = nodes[next_node_id].root();
        } while (next_node_id != StepNode<Step>::ROOT);

        return solution.finish(max_level, nodes.size(), taboo->size());
    }

    friend std::ostream& operator<<(std::ostream& os, const Solver<Step>& s) {
        os << *(s.game);
        return os;
    }

private:
    Solver();
    Solver(const Solver&);
    Solver& operator=(const Solver&);

    /// Adds new node with new valid steps as leaves
    int next_node(int node_id) {
        Log log(Log::DEBUG);
        auto steps = game->valid_steps();
        if (properties.filter)
            filter_steps(steps);
        int last_node_id = nodes.size() + steps.size();
        int next_node_id = nodes.size() + 1;

        log << "L" << level << " s:" << steps.size() << " ";
        std::for_each(steps.begin(), steps.end(), [&] (const Step& s) {
            log << s;
            if (next_node_id == last_node_id)
                next_node_id = StepNode<Step>::END;
            nodes.emplace_back(node_id, next_node_id++, s);
        });
        log << " s:" << nodes.size() << "-" << steps.size();

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
        Log(Log::DEBUG) << "undo: " << nodes[node_root].step() << std::endl;
        game->undo_step(nodes[node_root].step());
        --level;
        return next_node_id;
    }

    void filter_steps(std::vector<Step>& steps) {
        std::vector<Step> tmp;
        std::copy_if(steps.begin(), steps.end(), std::back_inserter(tmp), [=](const Step& s) -> bool {
            return previous_steps.find(s) == previous_steps.end();
        });
        // std::remove_if(steps.begin(), steps.end(), [=](const Step& s) -> bool {
        //     return previous_steps.find(s) != previous_steps.end();
        // });
        steps = tmp;
        previous_steps.insert(steps.begin(), steps.end());
    }

    SolverProperties properties;
    GamePtr<Step> game;
    std::vector<StepNode<Step>> nodes;
    std::unordered_set<Step, typename Step::Hash> previous_steps;
    std::shared_ptr<Taboo> taboo;
    unsigned level, max_level;
};

#endif