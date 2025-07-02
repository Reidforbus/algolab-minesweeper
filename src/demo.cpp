#include <iostream>
#include "minesweeper/aicore.hpp"
#include "minesweeper/game.hpp"
#include "minesweeper/tui.hpp"

bool play_demo_game(int r, int c, int m, uint32_t seed, bool seeded, std::string start_message = ""){
    algolab::Minegame game;
    algolab::GameTUI tui;
    game.new_game(r, c, m, seed, seeded);
    algolab::CSPAI ai(r, c, m);
    tui.new_game(r, c);
    tui.add_message(start_message);

    while (!game.game_finished()){
        auto state = game.get_state();
        tui.update_state(state);
        tui.draw();
        tui.clear_messages();

        ai.update_state(state);
        auto move = ai.get_next_move();
        getchar();
        if (move.guess){
            tui.add_message("AI made a guess");
        }
        if (!game.execute_move(move)){
            std::cout << "Something failed" << std::endl;
            return false;
        }

    }

    auto state = game.get_state();
    tui.update_state(state);
    tui.draw();

    return game.was_won();
}

int main(int argc, char** argv){
    std::cout << "Welcome to the demo of minesweeper!" << std::endl;

    // Game 1
    while (!play_demo_game(9, 9, 10, 0, false, "Playing game 1")){
    }

    // Game 2
    while (!play_demo_game(16, 16, 40, 0, false, "Playing game 2")){
    }

    // Game 3
    while (!play_demo_game(16, 30, 99, 0, false, "Playing game 3")){
    }
}
