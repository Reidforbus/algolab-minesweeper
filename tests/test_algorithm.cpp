#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <vector>
#include "minesweeper/game.hpp"
#include "minesweeper/aicore.hpp"

template<typename T>
class BenchmarkQueue {
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cond;

    public:
    void push(T val){
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(val);
        cond.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, [&]{return !queue.empty();});
        T val = queue.front();
        queue.pop();
        return val;
    }
};

inline bool run_game(int h = 16, int w = 30, int m = 99){
    algolab::Minegame game;
    game.new_game(h, w, m, 0, false);

    algolab::CSPAI ai(h, w, m);
    algolab::Move last_move;
    while (!game.game_finished()){
        auto state = game.get_state();
        ai.update_state(state);

        last_move = ai.get_next_move();
        if (last_move.action == algolab::OPEN){
            game.open(last_move.coord.row, last_move.coord.col);
        } else if (last_move.action == algolab::FLAG){
            game.flag(last_move.coord.row, last_move.coord.col);
        } else {
            FAIL();
        }
    }
    if (game.was_won()){
        return true;
    } else if (!game.was_won() && last_move.guess){
        return true;
    } else {
        return false;
    }
}

TEST_CASE("Testing ai speed", "[Algorithm] [!benchmark]"){
    BENCHMARK("Easy game"){
        return run_game(9,9,10);
    };

    BENCHMARK("Intermediate game"){
        return run_game(16,16,40);
    };
}

TEST_CASE("No false moves are made in 100 games", "[Algorithm][!benchmark]"){
    int number_of_games = 100;
    BenchmarkQueue<bool> result_queue;
    std::vector<std::future<void>> futures;

    for (int id = 0; id < number_of_games; id++){
        futures.push_back(std::async(std::launch::async, 
                [&result_queue](){
                    bool result = run_game();
                    result_queue.push(result);
                }));
    }


    for (int complete = 0; complete < number_of_games; complete++){
        bool result = result_queue.pop();
        REQUIRE(result);
        std::cout << complete + 1 << " out of 100 games done" << std::endl;
    }

    for (auto& fut : futures){
        fut.get();
    }
}
