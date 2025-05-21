#pragma once

#include <tuple>
#include <vector>

namespace algolab{
    struct Coord {
        int row;
        int col;

        Coord(int r = 0, int c = 0):row(r), col(c){}

        void operator +=(const Coord& a){
            row += a.row;
            col += a.col;
        }

        bool operator <(const Coord& a) const{
            return std::tie(row, col) < std::tie(a.row, a.col);
        }

        std::vector<Coord> neighbours(){
            std::vector<Coord> result = {{0,1},{1,1},{1,0},{0,-1},{-1,0},{-1,-1},{1,-1},{-1,1}};
            for (auto& sq : result){
                sq += *this;
            }
            return result;
        }

    };

    enum Move_action {
        OPEN,
        FLAG
    };

    struct Move {
        Coord coord;
        Move_action action; 
        bool guess;
    };
}
