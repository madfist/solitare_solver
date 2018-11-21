#ifndef PARALLEL_SOLVER_HEADER
#define PARALLEL_SOLVER_HEADER

#include "Game.hpp"
#include "ctpl_stl.hpp"
#include "shared_mutex.hpp"

template<class GameType> class ParallelSolver;

static std::mutex output_mutex;

template<class Step>
Solution<Step> thread_fn(int id, std::shared_ptr<Solver<Step>> solver) {
    {
        std::unique_lock<std::mutex> lock(output_mutex);
        std::cout << "thread" << id << '\n' << *solver << '\n';
    }
    return solver->solve();
    // {
    //     std::unique_lock<std::mutex> lock(output_mutex);
    //     std::cout << '\t' << id << ": " << p << '\n';
    // }
    // GameType temp_game(*(solver->game_src));
    // temp_game.do_steps(steps);
    // if (temp_game.win()) {
    //     std::cout << "WIN";
    //     solver->thread_pool.stop();
    // }
    // if (!solver->is_taboo(temp_game)) {
    //     solver->add_taboo(temp_game);
    //     solver->do_steps(steps, temp_game.valid_steps());
    // }
}

template<class GameType>
class ParallelSolver {
public:
    ParallelSolver(std::shared_ptr<GameType> g, SolverProperties p = {false}) : game(g), thread_pool(8), properties(p) {}

    typedef typename GameType::step_type Step;

    Solution<Step> solve() {
        Solution<Step> solution;
        if (!game->sanity())
            return solution.finish(0, 0, 0);
        auto taboo = std::make_shared<ConcurrentTaboo>();
        auto first_steps = game->valid_steps();
        std::vector<std::future<Solution<Step>>> results(first_steps.size());
        unsigned i = 0;
        for (auto step : first_steps) {
            auto g = std::make_shared<GameType>(*game);
            g->do_step(step);
            auto solver = std::make_shared<Solver<Step>>(g, taboo);
            results[i++] = std::move(thread_pool.push(thread_fn<Step>, solver));
        }
        auto status(std::future_status::timeout);
        int result_holder = -1;
        while (status != std::future_status::ready) {
            for (int r = 0; r < results.size(); ++r) {
                status = results[r].wait_for(std::chrono::milliseconds(100));
                if (status == std::future_status::ready) {
                    result_holder = r;
                    break;
                }
            }
        }
        thread_pool.stop();
        return results[result_holder].get();
    }

private:
    std::shared_ptr<GameType> game;
    ctpl::thread_pool thread_pool;
    SolverProperties properties;
//     ParallelSolver(std::shared_ptr<GameType> g) : game_src(g), thread_pool(8), progress(0) {}

//     typedef typename GameType::step_type Step;

//     Solution<Step> solve() {
//         std::vector<Step> empty;
//         Solution<Step> solution;
//         do_steps(empty, game_src->valid_steps());
//         return solution;
//     }

//     void do_steps(const std::vector<Step>& steps, const std::vector<Step>& valid_steps) {
//         for (auto s = valid_steps.begin(); s != valid_steps.end(); ++s) {
//             auto ss(steps);
//             ss.push_back(*s);
//             thread_pool.push(thread_fn<GameType, Step>, this, ss, progress);
//             {
//                 std::unique_lock<std::mutex> lock(progress_mutex);
//                 progress++;
//                 if (progress%10 == 0) {
//                     std::unique_lock<std::mutex> lock(output_mutex);
//                     std::cout << progress << '\n';
//                 }
//             }
//         }
//     }

//     template<class G, class Step>
//     friend void thread_fn(int id, ParallelSolver<G>* solver, const std::vector<Step>& steps, uint64_t p);
// private:
//     bool is_taboo(const GameType& game) const {
//         ting::shared_lock<ting::shared_mutex> lock(taboo_mutex);
//         return (taboo.find(game.hash()) != taboo.end());
//     }

//     void add_taboo(const GameType& game) {
//         std::unique_lock<ting::shared_mutex> lock(taboo_mutex);
//         taboo.insert(game.hash());
//     }

//     std::shared_ptr<GameType> game_src;
//     std::unordered_set<std::size_t> taboo;
//     ctpl::thread_pool thread_pool;
//     mutable ting::shared_mutex taboo_mutex;
//     std::mutex progress_mutex;
//     uint64_t progress;
};

#endif