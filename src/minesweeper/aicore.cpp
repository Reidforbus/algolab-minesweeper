#include "minesweeper/aicore.hpp"
#include <random>

namespace algolab{
            bool CSPAI::in_bounds(const Coord& sq){
                return (sq.row >= 0) && (sq.row < height) && (sq.col >= 0) && (sq.col < width);
            }

            bool CSPAI::calculate_moves(){
                bool progressed = false;

                while (!changed.empty()){
                    Coord open_sq = changed.back();
                    changed.pop_back();

                    int value = opened[open_sq];
                    int remaining_mines = mine_count - mines.size();
                    csp.update_remaining_ones(remaining_mines);
                    if (remaining_mines == 0){
                        for (auto sq : unknown){
                            to_open.push(sq);
                        }
                        return true;
                    }
                    if (value != 0) {

                        std::vector<Coord> nbrs = open_sq.neighbours();
                        std::vector<Coord> variables;

                        for (Coord sq : nbrs){
                            if (in_bounds(sq) && (opened.count(sq) == 0)){
                                variables.push_back(sq);
                            }
                        }
                        csp.add_constraint(variables, value);
                    }
                    csp.add_opened(open_sq);
                }

                while (true){
                    auto [sq, value] = csp.get_solution();
                    if (value < 0){
                        break;
                    } else if (value == 0) {
                        to_open.push(sq);
                    } else if (value == 1) {
                        mines.insert(sq);
                        unknown.erase(sq);
                        to_flag.push(sq);
                    }

                    progressed = true;
                }

                return progressed;
            }

            bool CSPAI::make_guess(){
                const Coord corners[] = {{0, 0}, {0, width - 1}, {height - 1, 0}, {height - 1, width - 1}};
                for (Coord sq : corners){
                    if (unknown.count(sq)){
                        to_open.push(sq);
                        return true;
                    }
                }

                if (!unknown.empty()){
                    std::uniform_int_distribution<> distributor(0, unknown.size()-1);
                    std::random_device rd;
                    std::mt19937 rng(rd());
                    uint32_t idx = distributor(rng);
                    auto iter = unknown.cbegin();
                    for (int i = 0; i < idx; i++){
                        iter++;
                    }
                    to_open.push(*iter);
                    return true;
                }

                return false;
            }

            CSPAI::CSPAI(int h, int w, int m):
                height(h), width(w), mine_count(m){
                    for (auto j = 0;j < height; j++){
                        for (auto i = 0;i < width; i++){
                            Coord sq(j, i);
                            unknown.insert(sq);
                        }
                    }
                }

            void CSPAI::update_state(std::vector<int> state){
                for (auto j = 0;j < height; j++){
                    for (auto i = 0;i < width; i++){
                        Coord sq(j, i);
                        if (opened.count(sq)){
                            continue;
                        }
                        auto val = state[(j * width) + i] % 16;
                        if (val < 9){
                            changed.push_back(sq);
                            opened[sq] = val;
                            unknown.erase(sq);
                        }
                    }
                }
            }

            Move CSPAI::get_next_move(){

                while (true) {
                    if (!to_flag.empty()){
                        Move next = {to_flag.front(), FLAG, false};
                        to_flag.pop();
                        return next;
                    }

                    while (!to_open.empty()){
                        Move next = {to_open.front(), OPEN, false};
                        to_open.pop();
                        if (opened.count(next.coord) > 0) continue;
                        return next;
                    }

                    bool found_moves = calculate_moves();


                    if (!found_moves){
                        if (csp.brute_force_search()){
                            continue;
                        }
                        if (make_guess()){
                            Move guess = {to_open.front(), OPEN, true};
                            to_open.pop();
                            return guess;
                        }

                        break;
                    }

                }
                return Move{Coord(), FAIL, false};
            }


} // namespace algolab
