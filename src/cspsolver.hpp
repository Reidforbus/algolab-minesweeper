#include <cstdint>
#include <map>
#include <set>
#include "utilities.hpp"

namespace algolab{
    class CSPAI{
        private:
            uint32_t height, width, mine_count;
            std::set<Coord> mines;
            std::map<Coord, uint32_t> opened;
            std::vector<Coord> changed;
            std::vector<Coord> to_open;
            std::vector<Coord> to_flag;

            bool in_bounds(const Coord& sq){
                return (sq.row >= 0 & sq.row < height & sq.col >= 0 & sq.col < width);
            }

            bool all_free_neighbors(const Coord& coord){
                // TODO
                return false;
            }

            bool all_mined_neighbors(const Coord& coord){
                // TODO
                return false;
            }

            void calculate_moves(){

            }

        public:
            CSPAI(uint32_t h, uint32_t w, uint32_t m):
                height(h), width(w), mine_count(m){}

            void update_state(std::vector<uint32_t> state){
                for (auto j = 0;j < height; j++){
                    for (auto i = 0;i < width; i++){
                        Coord sq = {j, i};
                        if (mines.count(sq) | opened.count(sq)){
                            continue;
                        }
                        auto val = state[(j * width) + i];
                        if (val < 9){
                            changed.push_back(sq);
                            opened[sq] = val;
                        }
                    }
                }
            }



    };
} // namespace algolab
