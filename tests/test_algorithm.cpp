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

bool run_game(int h = 16, int w = 30, int m = 99){
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

bool no_guesses_and_won(int h, int w, int m, algolab::Minegame& game){

    algolab::CSPAI ai(h, w, m);
    algolab::Move last_move;

    while (!game.game_finished()){
        auto state = game.get_state();
        ai.update_state(state);

        last_move = ai.get_next_move();
        if (last_move.guess) return false;
        if (!game.execute_move(last_move)){
            FAIL();
        }
    }
    return game.was_won();
}


TEST_CASE("'No guesses' game 1", "[Validation]"){
    algolab::Minegame game;
    game.from_string(16, 16, 
            "#*####**##*#####"
            "*#*##*#####*####"
            "*#####*#*#####*#"
            "####*###########"
            "#####*#*########"
            "#*###***####*###"
            "#**##*##*#*#####"
            "#**#####**##***#"
            "######*#########"
            "#########*###*##"
            "###**##*###**#*#"
            "##*########*###*"
            "######*##*#####*"
            "##*#####*##*####"
            "############*#*#"
            "####*#####*##*##"
            );
    game.open(15, 2);
    REQUIRE(no_guesses_and_won(16, 16, 55, game));
}

TEST_CASE("'No guesses' game 2", "[Validation]"){
    algolab::Minegame game;
    game.from_string(16, 16, 
            "####*#*####*####"
            "###**#########*#"
            "*#*#####**######"
            "##*######*######"
            "########*##**###"
            "######*#*##*##*#"
            "*##*#####*######"
            "#*###*#*##*#*###"
            "###*#####**#*##*"
            "###*#########*##"
            "#*#*####*###*###"
            "*#####**####*##*"
            "#**##**###*#*###"
            "###*##*########*"
            "*##*#*****###*##"
            "*####*###**##*##"
            );
    game.open(3, 5);
    REQUIRE(no_guesses_and_won(16, 16, 65, game));
}

TEST_CASE("'No guesses' game 3", "[Validation]"){
    algolab::Minegame game;
    game.from_string(16, 16, 
            "###*#**###*##*##"
            "*######*##*##*##"
            "*########*##**#*"
            "####***####**###"
            "########*#####*#"
            "*#####**####*###"
            "###*#*##########"
            "#########***####"
            "#*########**#***"
            "#**##*##**####**"
            "####*#######**##"
            "**##########**##"
            "##*#*##***######"
            "########*##**##*"
            "####**#*###*####"
            "**#**##*######**"
            );
    game.open(6, 14);
    REQUIRE(no_guesses_and_won(16, 16, 70, game));
}

TEST_CASE("'No guesses' game 4", "[Validation]"){
    algolab::Minegame game;
    game.from_string(16, 16, 
            "*####**##*****#*"
            "**#############*"
            "*#*###*##*####**"
            "#*#####*########"
            "##*####**####*##"
            "####*##*#######*"
            "#*#*##*###**###*"
            "*####***#####*#*"
            "######*######*#*"
            "*####*##*#*####*"
            "###***#######***"
            "###*##*#####*###"
            "*####*###*#*##*#"
            "**####*###*####*"
            "#**#*##*##*###*#"
            "#######***#####*"
            );
    game.open(8, 2);
    REQUIRE(no_guesses_and_won(16, 16, 76, game));
}

TEST_CASE("'No guesses' game 5", "[Validation]"){
    algolab::Minegame game;
    game.from_string(10, 10, 
            "#*#**#*#**"
            "###*#*##*#"
            "#####*#*#*"
            "###*######"
            "##*##*###*"
            "######*#*#"
            "**#######*"
            "#*#*####**"
            "*#####*##*"
            "###****##*"
            );
    game.open(2, 0);
    REQUIRE(no_guesses_and_won(10, 10, 34, game));
}

TEST_CASE("'No guesses' game 6", "[Validation]"){
    algolab::Minegame game;
    game.from_string(20, 30, 
            "*####*#*##########*#*#########"
            "#*################*######*####"
            "#########*#######**######*####"
            "#**##*##########****#**##*##*#"
            "*###*#*######****######*###*##"
            "######*#****####**#*####*#####"
            "#*#########*##*####*#*######**"
            "#########*###*####*###########"
            "#######*#######*##*####*######"
            "###*####*#**##*########*####**"
            "##*####*#*##*#*##*############"
            "####*#####*##*#####*#####**###"
            "##*#*###***####*############**"
            "#*#############*###**#*#*###*#"
            "**######*##*##*###*####*#*####"
            "################*#*###########"
            "###*############*###*#####***#"
            "**########*#####*###*#*###*###"
            "#*######*###########**########"
            "#*#***###****#########**#**##*"
            );
    game.open(19, 15);
    REQUIRE(no_guesses_and_won(20, 30, 130, game));
}

TEST_CASE("No false moves are made in 100 games", "[Validation]"){
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
        std::cout << "\r" <<  complete + 1 << " out of 100 games done" << std::flush;
    }
    std::cout << std::endl;

    for (auto& fut : futures){
        fut.get();
    }
}

TEST_CASE("Testing ai speed", "[!benchmark]"){
    BENCHMARK("Easy game"){
        return run_game(9,9,10);
    };

    BENCHMARK("Intermediate game"){
        return run_game(16,16,40);
    };

    BENCHMARK("Expert game"){
        return run_game(16,30,99);
    };
}
