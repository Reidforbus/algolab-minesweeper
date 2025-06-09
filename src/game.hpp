#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#include "utilities.hpp"

namespace algolab{
    class Minegame{
        struct Square{
            bool mine;
            bool open = false;
            bool flagged = false;
            uint32_t adjacent_mines = 0;
            Coord coord;
        };

        private:

        uint32_t height;
        uint32_t width;
        uint32_t mine_count;
        std::vector<std::vector<Square>> board;
        bool game_over;
        Coord last_move{-1,-1};

        
        void game_lost(){
            game_over = true;
            std::cout << "GAME OVER" << std::endl;
            for (auto j = 0; j < height; j++){
                for (auto i = 0; i < width; i++){
                    board[j][i].open = true;
                }
            }
        }

        void game_won(){
            game_over = true;
            std::cout << "GAME WON :)" << std::endl;
        }


        bool check_board(){
            int opened = 0;
            for (auto j = 0; j < height; j++){
                for (auto i = 0; i < width; i++){

                    Square& sq = board[j][i];
                    if (sq.open && sq.mine){
                        game_lost();
                        return false;
                    } else if (sq.open){
                        opened++;
                    }
                }
            }
            if (opened == ((height * width) - mine_count)){
                game_won();
                return false;
            }
            return true;
        }

        bool in_bounds(int row, int col){
            return ((row >= 0) && (row < height) && (col >= 0) && (col < width));
        }

        void spread_open(uint32_t row, uint32_t col){
            uint32_t n = 0;
            std::vector<Coord> stack;
            std::set<Coord> seen;
            stack.push_back(Coord(row, col));

            while (!stack.empty()){
                Coord sq = stack.back();
                stack.pop_back();
                seen.insert(sq);
                Square* sq_ptr = &board[sq.row][sq.col];
                sq_ptr->open = true;
                if (sq_ptr->adjacent_mines == 0){
                    for (auto nbr : sq.neighbours()) {
                        if (in_bounds(nbr.row, nbr.col)){
                            if (!seen.count(nbr)){
                                stack.push_back(nbr);
                            }
                        }
                    }
                }
            }
        }

        public:

        bool game_finished(){
            return game_over;
        }

        void cede(){
            game_lost();
        }

        void new_game(uint32_t h, uint32_t w, uint32_t m, uint32_t seed, bool seeded){
            height = h;
            width = w;
            mine_count = m;
            game_over = false;
            board.clear();

            if (!seeded){
                seed = std::random_device()();
            }
            std::cout << "Playing with seed: " << seed << std::endl;
            std::mt19937 randomizer(seed);
            std::uniform_int_distribution<> distributor(0, (height * width) - 1);

            std::vector<bool> mines(height*width);
            for (int i = 0; i< mine_count;i++) {
                uint32_t idx = distributor(randomizer);
                if (mines[idx]) {
                    i--;
                } else {
                    mines[idx] = true;
                }
            }

            for (int y = 0; y < height; y++) {
                std::vector<Square> row;

                for (int x = 0; x < width; x++) {
                    Square next;
                    next.coord = {y, x};
                    next.mine = mines[(y*width) + x];
                    row.push_back(next);
                }
                board.push_back(row);
            }

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    Square* sq = &board[y][x];
                    if (sq->mine) {
                        for (auto nbr : sq->coord.neighbours()) {
                            //std::cout << nbr.row << " " << nbr.col << std::endl;
                            if (in_bounds(nbr.row, nbr.col)){
                                board[nbr.row][nbr.col].adjacent_mines++;
                            }
                        }
                    }
                }
            }
        }



        bool open(uint32_t row, uint32_t col) {
            if (!in_bounds(row, col)) {
                std::cout << "Was out of bounds" << std::endl;
                return true;
            }

            Square* slot = &board[row][col];
            if (slot->open){
                return true;
            }

            last_move = {int(row), int(col)};

            spread_open(row, col);
            return check_board();

        }

        std::vector<uint32_t> get_state() {
            std::vector<uint32_t> state;
            for (auto row : board) {
                for (Square sq : row) {
                    uint32_t x = 10;
                    if (sq.open && sq.mine) {
                        x = 9;
                    } else if (sq.open) {
                        x = sq.adjacent_mines;
                    } else if (sq.flagged) {
                        x = 11;
                    }
                    if (sq.coord == last_move){
                        x += 16;
                    }
                    state.push_back(x);
                }
            }
            return state;
        }

        void flag(uint32_t row, uint32_t col) {
            if (!in_bounds(row, col)){
                return;
            }

            Square* slot = &board[row][col];
            if (slot->open){
                return;
            }
            last_move = {int(row), int(col)};
            slot->flagged = !slot->flagged;
        }

        void print_state() {
            for (auto i : board) {
                for (Square j : i) {
                    if (j.mine) {
                        std::cout << "@";
                    } else {
                        std::cout << j.adjacent_mines;
                    }
                    std::cout << " ";
                }
                std::cout << std::endl;
            }
        }
    };
} //namespace algolab
