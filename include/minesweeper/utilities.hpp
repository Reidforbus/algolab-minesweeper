#pragma once

#include <tuple>
#include <vector>
#include <ostream>

namespace algolab{
    struct Coord {
        int row;
        int col;

        Coord(int r = 0, int c = 0):row(r), col(c){}

        Coord& operator +=(const Coord& a){
            row += a.row;
            col += a.col;
            return *this;
        }

        bool operator==(const Coord& a) const{
            return std::tie(row, col) == std::tie(a.row, a.col);
        }

        bool operator <(const Coord& a) const{
            return std::tie(row, col) < std::tie(a.row, a.col);
        }

        bool operator >(const Coord& a) const{
            return std::tie(row, col) > std::tie(a.row, a.col);
        }

        std::vector<Coord> neighbours() const{
            std::vector<Coord> result = {{0,1},{1,1},{1,0},{0,-1},{-1,0},{-1,-1},{1,-1},{-1,1}};
            for (auto& sq : result){
                sq += *this;
            }
            return result;
        }


    };

    inline std::ostream& operator<<(std::ostream& out, const Coord& a) {
        return out << "(" << a.row << ", " << a.col << ")";
    }

    enum Move_action {
        FAIL,
        OPEN,
        FLAG,
    };

    struct Move {
        Coord coord;
        Move_action action; 
        bool guess;

        bool operator==(const Move& a) const{
            return (coord == a.coord) && (action == a.action) && (guess == a.guess);
        }
    };
}
