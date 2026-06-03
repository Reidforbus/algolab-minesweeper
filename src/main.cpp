#include "minesweeper/game.hpp"
#include "minesweeper/tui.hpp"
#include "minesweeper/aicore.hpp"
#include "minesweeper/utilities.hpp"
#include <limits>
#include <thread>
#include <chrono>

#define BOARD_HEIGHT 16
#define BOARD_WIDTH 30
#define NUMBER_OF_MINES 80

void print_ascii_board(const std::vector<int>& state, int width, int height){
    std::cout << "\033[2J\033[H";
    std::cout << "\033[0m";
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            int val = state[(j * width) + i];
            int base = val % 16;
            bool is_last = val >= 16;

            if (is_last){
                std::cout << "\033[48;5;236m";
            } else if (base >= 10){
                std::cout << "\033[48;5;239m";
            } else {
                std::cout << "\033[48;5;235m";
            }

            if (base == 0){
                std::cout << "\033[38;5;246m\u00b7\u00b7\033[0m";
            } else if (base < 9 && base > 0){
                std::cout << "\033[38;5;246m\u00b7\033[0m";
                switch(base){
                    case 1: std::cout << "\033[38;5;33m" << base << "\033[0m"; break;
                    case 2: std::cout << "\033[38;5;40m" << base << "\033[0m"; break;
                    case 3: std::cout << "\033[38;5;196m" << base << "\033[0m"; break;
                    case 4: std::cout << "\033[38;5;163m" << base << "\033[0m"; break;
                    case 5: std::cout << "\033[38;5;214m" << base << "\033[0m"; break;
                    case 6: std::cout << "\033[38;5;51m" << base << "\033[0m"; break;
                    default: std::cout << "\033[38;5;250m" << base << "\033[0m"; break;
                }
            } else if (base == 9){
                std::cout << "\033[38;5;196m\u00b7*\033[0m";
            } else if (base == 10){
                std::cout << "\033[38;5;240m\u00b7?\033[0m";
            } else if (base == 11){
                std::cout << "\033[38;5;214m\u00b7F\033[0m";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\033[0m";
}

void play_game(int board_height, int board_width, int mine_count, uint32_t seed, bool seeded, bool ascii_mode, int delay_ms){

    algolab::Minegame game;
    algolab::GameTUI tui;
    game.new_game(board_height, board_width, mine_count, seed, seeded);

    algolab::CSPAI ai(board_height, board_width, mine_count);

    while (!game.game_finished()){
        auto new_state = game.get_state();
        ai.update_state(new_state);

        if (ascii_mode){
            print_ascii_board(new_state, board_width, board_height);
        } else {
            tui.new_game(board_height, board_width);
            tui.update_state(new_state);
            tui.draw();
        }

        if (delay_ms > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }

        auto next_move = ai.get_next_move();

        if (next_move.action != algolab::FAIL){
            if (delay_ms <= 0){
                if (next_move.guess){
                std::cout << "AI is making a guess. Press enter to play" << std::endl;
                } else {
                std::cout << "AI found a move. Press enter to play" << std::endl;
                }
                getchar();
            }

            if (next_move.action == algolab::OPEN){
                game.open(next_move.coord.row, next_move.coord.col);
            } else if (next_move.action == algolab::FLAG){
                game.flag(next_move.coord.row, next_move.coord.col);
            }

        } else {

            if (delay_ms <= 0){
                std::cerr << "AI couldn't make a move, can you help?" << std::endl;
                int row, col;
                char command = 'x';
                while (true) {
                    std::cin >> command;
                    if (command == 'q') {
                        game.cede();
                        break;
                    }
                    std::cin >> row >> col;
                    if (std::cin.fail()) {
                        std::cout << "Could not parse command. Try again." << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }

                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if (command == 'f') {
                        game.flag(row - 1, col - 1);
                    } else if (command == 'o') {
                        game.open(row - 1, col - 1);
                    } else if (command == 'q'){
                        game.cede();
                    }

                    break;
                }
            } else {
                std::cerr << "AI couldn't make a move, ceding." << std::endl;
                game.cede();
            }
        }
    }
    auto new_state = game.get_state();
    if (ascii_mode){
        print_ascii_board(new_state, board_width, board_height);
    } else {
        algolab::GameTUI tui;
        tui.new_game(board_height, board_width);
        tui.update_state(new_state);
        tui.draw();
    }
}

int main(int argc, char** argv) {

    int i = 1;
    int seed = 0;
    bool seeded = false;
    bool ascii_mode = false;
    int delay_ms = 0;
    while (i < argc){
        std::string flag(argv[i++]);
        if (flag.compare("-s") == 0 && i < argc){
            try {
                seed = std::stoul(std::string(argv[i]));
                seeded = true;
            } catch (std::invalid_argument) {
                std::cerr << "Invalid seed. Please give a whole number" << std::endl;
                return EXIT_FAILURE;
            } catch (std::out_of_range) {
                std::cerr << "Seed is too big. It can't be more than " << ((uint32_t)0 - 1) << std::endl;
                return EXIT_FAILURE;
            }
        } else if (flag.compare("-a") == 0){
            ascii_mode = true;
        } else if (flag.compare("-d") == 0 && i < argc){
            try {
                delay_ms = std::stoi(std::string(argv[i]));
            } catch (std::invalid_argument) {
                std::cerr << "Invalid delay. Please give a whole number in milliseconds" << std::endl;
                return EXIT_FAILURE;
            }
            i++;
        }
    }


    play_game(BOARD_HEIGHT, BOARD_WIDTH, NUMBER_OF_MINES, seed, seeded, ascii_mode, delay_ms);

    return EXIT_SUCCESS;
}
