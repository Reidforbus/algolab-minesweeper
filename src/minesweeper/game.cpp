#include "minesweeper/game.hpp"
#include <iostream>
#include <set>
#include <random>

namespace algolab{

    Minegame::Minegame(){
        last_move = {-1,-1};
        seed = 0;
    }


    void Minegame::game_lost(){
        game_over = true;
        for (auto j = 0; j < height; j++){
            for (auto i = 0; i < width; i++){
                board[j][i].open = true;
            }
        }
    }

    void Minegame::game_won(){
        game_over = true;
        winner = true;
    }


    bool Minegame::check_board(){
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

    bool Minegame::in_bounds(int row, int col){
        return ((row >= 0) && (row < height) && (col >= 0) && (col < width));
    }

    void Minegame::spread_open(int row, int col){
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

    void Minegame::fill_board(std::vector<bool> mines){
        board = std::vector<std::vector<Square>>(height, std::vector<Square>(width));
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Square& sq = board[y][x];
                sq.coord = {y, x};
                sq.mine = mines[(y*width) + x];
            }
        }
        fill_numbers();
    }

    void Minegame::fill_numbers(){
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Square& sq = board[y][x];
                if (sq.mine) {
                    for (auto nbr : sq.coord.neighbours()) {
                        if (in_bounds(nbr.row, nbr.col)){
                            board[nbr.row][nbr.col].adjacent_mines++;
                        }
                    }
                }
            }
        }
    }


    bool Minegame::game_finished(){
        return game_over;
    }

    bool Minegame::was_won(){
        return winner;
    }

    void Minegame::cede(){
        game_lost();
    }

    uint32_t Minegame::get_seed(){
        return seed;
    }

    void Minegame::from_string(int h, int w, std::string input){
        height = h;
        width = w;
        int m = 0;
        std::vector<bool> minevector(height*width);
        for (int i = 0; i < input.length(); i++){
            if (input[i] == '*'){
                m++;
                minevector[i] = true;
            }
        }

        if (h <= 0 || w <= 0 || m <= 0 || m >= (h * w) || input.length() != (h*w)){
            throw std::invalid_argument("Parameters were invalid");
        }
        mine_count = m;
        game_over = false;
        winner = false;
        board.clear();
        seed = 0;

        fill_board(minevector);
    }

    void Minegame::from_coords(int h, int w, std::vector<Coord> mines){
        height = h;
        width = w;
        mine_count = mines.size();
        if (h <= 0 || w <= 0 || mine_count <= 0 || mine_count >= (h * w)){
            throw std::invalid_argument("Parameters were invalid");
        }
        game_over = false;
        winner = false;
        board.clear();
        seed = 0;

        std::vector<bool> minevector(height*width);

        for (auto mine : mines){
            minevector[mine.row * width + mine.col] = true;
        }

        fill_board(minevector);
    }

    void Minegame::new_game(int h, int w, int m, uint32_t new_seed, bool seeded){
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

        fill_board(mines);

    }

    void Minegame::open(int row, int col) {
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

    std::vector<int> Minegame::get_state() {
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

    void Minegame::flag(int row, int col) {
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

    bool Minegame::execute_move(algolab::Move move){
        if (move.action == FAIL){
            return false;
        }
        if (move.action == OPEN){
            open(move.coord.row, move.coord.col);
            return true;
        } else if (move.action == FLAG){
            flag(move.coord.row, move.coord.col);
            return true;
        }
        return false;
    }

    void Minegame::print_state(std::ostream& output) {
        for (auto i : board) {
            for (Square j : i) {
                if (j.mine) {
                    output << "@";
                } else {
                    output << j.adjacent_mines;
                }
                output << " ";
            }
            output << std::endl;
        }
    }
}
