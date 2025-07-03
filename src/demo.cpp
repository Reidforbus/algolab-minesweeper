#include <chrono>
#include <iostream>
#include <termios.h>
#include <thread>
#include <unistd.h>

#include "minesweeper/aicore.hpp"
#include "minesweeper/game.hpp"
#include "minesweeper/tui.hpp"

void wait_for_key(){
        // Save old terminal settings
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Wait for a single character
    char ch;
    read(STDIN_FILENO, &ch, 1);

    // Restore old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    std::cout << std::endl;
}

bool play_demo_game(int r, int c, int m, uint32_t seed, bool seeded, std::string start_message = "", bool wait = true){
    algolab::Minegame game;
    algolab::GameTUI tui;
    game.new_game(r, c, m, seed, seeded);
    algolab::CSPAI ai(r, c, m);
    tui.new_game(r, c);
    tui.add_message(start_message);
    tui.add_message("Playing with seed " + std::to_string(game.get_seed()));

    while (!game.game_finished()){
        auto state = game.get_state();
        tui.update_state(state);
        tui.draw();

        ai.update_state(state);
        auto move = ai.get_next_move();
        if (wait) {
            wait_for_key();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
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

    play_demo_game(8, 8, 6, 105520637, true);
    wait_for_key();

    play_demo_game(8, 8, 9, 3633124087, true);
    wait_for_key();

    play_demo_game(8, 8, 12, 633059783, true);
    wait_for_key();

    // Game 1
    while (!play_demo_game(9, 9, 10, 0, false, "Playing easy game")){
        std::cout << std::endl;
        wait_for_key();
    }
    std::cout << std::endl;
    wait_for_key();

    // Game 2
    while (!play_demo_game(16, 16, 40, 0, false, "Playing intermediate game")){
        std::cout << std::endl;
        wait_for_key();
    }
    std::cout << std::endl;
    wait_for_key();

    // Game 3
    while (true) {
        while (!play_demo_game(16, 30, 99, 0, false, "Playing expert game", false)){
            std::cout << std::endl;
            wait_for_key();
        }
    }
    std::cout << std::endl;
}
