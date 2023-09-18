#ifndef PARALLEL_SOLVER_HEADER
#define PARALLEL_SOLVER_HEADER

#include "Game.hpp"
#include "Log.hpp"
#include "Trace.hpp"
#include "ctpl_stl.hpp"

template<class GameType> class ParallelSolver;

static std::mutex output_mutex;

template<class Step>
Solution<Step> thread_fn(int id, std::shared_ptr<Solver<Step>> solver) {
    {
        std::unique_lock<std::mutex> lock(output_mutex);
        Trace(TraceComponent::PARALLEL) << "thread" << id << '\n' << *solver;
    }
    return solver->solve();
}

template<class GameType>
class ParallelSolver {
public:
    ParallelSolver(std::shared_ptr<GameType> g, SolverProperties p = {false}) : game(g), thread_pool(8), properties(p) {}

    typedef typename GameType::step_type Step;

    Solution<Step> solve() {
        Solution<Step> solution;
        if (!game->sanity())
            return solution.finish(false, 0, 0, 0);
        auto taboo = std::make_shared<ConcurrentTaboo<Step>>();
        auto first_steps = game->valid_steps();
        Log(Log::INFO) << "Started " << first_steps.size() << " threads";
        std::vector<std::future<Solution<Step>>> results(first_steps.size());
        unsigned i = 0;
        for (auto step : first_steps) {
            auto g = std::make_shared<GameType>(*game);
            g->do_step(step);
            auto solver = std::make_shared<Solver<Step>>(g, taboo);
            results[i++] = std::move(thread_pool.push(thread_fn<Step>, solver));
        }
        int result_holder = -1;
        bool all_done = false;
        while (!all_done) {
            all_done = true;
            for (int r = 0; r < results.size(); ++r) {
                auto status = results[r].wait_for(std::chrono::milliseconds(100));
                all_done &= (status == std::future_status::ready);
            }
        }
        thread_pool.stop();
        for (int r = 0; r < results.size(); ++r) {
            auto s = results[r].get();
            Log(Log::INFO) << "Partial result[" << r << "]: " << s;
            solution += s;
        }
        solution.set_taboo_tree_size(taboo->size());
        return solution;
    }

private:
    std::shared_ptr<GameType> game;
    ctpl::thread_pool thread_pool;
    SolverProperties properties;
};

#endif
