#include <cstdint>
#include <map>
#include <set>
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

        std::vector<Coord> neighbours(const Coord& a){
            std::vector<Coord> result = {{0,1},{1,1},{1,0},{0,-1},{-1,0},{-1,-1},{1,-1},{-1,1}};
            for (Coord sq : result){
                sq += a;
            }
            return result;
        }

    };

    class CSPAI{
        private:
            uint32_t height, width, mine_count;
            std::set<Coord> mines;
            std::map<Coord, uint32_t> opened;
            std::vector<Coord> changed;
            std::vector<Coord> to_open;
            std::vector<Coord> to_flag;

            uint64_t hash_coord(Coord sq){
                return (sq.row * width) + sq.col;
            }

            bool in_bounds(Coord sq){
                return (sq.row >= 0 & sq.row < height & sq.col >= 0 & sq.col < width);
            }

            bool all_free_neighbors(Coord coord){
                // TODO
                return false;
            }

            bool all_mined_neighbors(Coord coord){
                // TODO
                return false;
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
