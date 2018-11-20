#ifndef PARALLEL_SOLVER_HEADER
#define PARALLEL_SOLVER_HEADER

#include "Game.hpp"
#include "ctpl_stl.hpp"
#include "shared_mutex.hpp"

template<class GameType> class ParallelSolver;

static std::mutex output_mutex;

template<class GameType, class Step>
void thread_fn(int id, ParallelSolver<GameType>* solver, const std::vector<Step>& steps, uint64_t p) {
    {
        std::unique_lock<std::mutex> lock(output_mutex);
        std::cout << '\t' << id << ": " << p << '\n';
    }
    GameType temp_game(*(solver->game_src));
    temp_game.do_steps(steps);
    if (temp_game.win()) {
        std::cout << "WIN";
        solver->thread_pool.stop();
    }
    if (!solver->is_taboo(temp_game)) {
        solver->add_taboo(temp_game);
        solver->do_steps(steps, temp_game.valid_steps());
    }
}

template<class GameType>
class ParallelSolver {
public:
    ParallelSolver(std::shared_ptr<GameType> g) : game_src(g), thread_pool(8), progress(0) {}

    typedef typename GameType::step_type Step;

    Solution<Step> solve() {
        std::vector<Step> empty;
        Solution<Step> solution;
        do_steps(empty, game_src->valid_steps());
        return solution;
    }

    void do_steps(const std::vector<Step>& steps, const std::vector<Step>& valid_steps) {
        for (auto s = valid_steps.begin(); s != valid_steps.end(); ++s) {
            auto ss(steps);
            ss.push_back(*s);
            thread_pool.push(thread_fn<GameType, Step>, this, ss, progress);
            {
                std::unique_lock<std::mutex> lock(progress_mutex);
                progress++;
                if (progress%10 == 0) {
                    std::unique_lock<std::mutex> lock(output_mutex);
                    std::cout << progress << '\n';
                }
            }
        }
    }

    template<class G, class Step>
    friend void thread_fn(int id, ParallelSolver<G>* solver, const std::vector<Step>& steps, uint64_t p);
private:
    bool is_taboo(const GameType& game) const {
        ting::shared_lock<ting::shared_mutex> lock(taboo_mutex);
        return (taboo.find(game.hash()) != taboo.end());
    }

    void add_taboo(const GameType& game) {
        std::unique_lock<ting::shared_mutex> lock(taboo_mutex);
        taboo.insert(game.hash());
    }

    std::shared_ptr<GameType> game_src;
    std::unordered_set<std::size_t> taboo;
    ctpl::thread_pool thread_pool;
    mutable ting::shared_mutex taboo_mutex;
    std::mutex progress_mutex;
    uint64_t progress;
};

#endif