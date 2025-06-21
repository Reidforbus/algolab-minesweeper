#pragma once

#include "utilities.hpp"
#include <random>
#include <set>
#include <iostream>

namespace algolab{
    class Minegame{
        friend class MinegameTest;

        struct Square{
            bool mine;
            bool open = false;
            bool flagged = false;
            int adjacent_mines = 0;
            Coord coord;
        };

        private:

        uint32_t seed = 0;
        int height;
        int width;
        int mine_count;
        std::vector<std::vector<Square>> board;
        bool game_over;
        bool winner;
        Coord last_move{-1,-1};


        void game_lost(){
            game_over = true;
            for (auto j = 0; j < height; j++){
                for (auto i = 0; i < width; i++){
                    board[j][i].open = true;
                }
            }
        }

        void game_won(){
            game_over = true;
            winner = true;
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

        void spread_open(int row, int col){
            int n = 0;
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

        bool was_won(){
            return winner;
        }

        void cede(){
            game_lost();
        }

        uint32_t get_seed(){
            return seed;
        }

        void new_game(int h, int w, int m, uint32_t new_seed, bool seeded){
            height = h;
            width = w;
            if (h <= 0 || w <= 0 || m <= 0 || m >= (h * w)){
                throw std::invalid_argument("Parameters were invalid");
            }
            mine_count = m;
            game_over = false;
            winner = false;
            board.clear();
            seed = new_seed;

            if (!seeded){
                seed = std::random_device()();
            }
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
                            if (in_bounds(nbr.row, nbr.col)){
                                board[nbr.row][nbr.col].adjacent_mines++;
                            }
                        }
                    }
                }
            }
        }

        void open(int row, int col) {
            if (!in_bounds(row, col)) {
                return;
            }

            Square* slot = &board[row][col];
            if (slot->open){
                return;
            }

            last_move = Coord{row, col};

            spread_open(row, col);
            check_board();

        }

        std::vector<int> get_state() {
            std::vector<int> state;
            for (auto row : board) {
                for (Square sq : row) {
                    int x = 10;
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

        void flag(int row, int col) {
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
