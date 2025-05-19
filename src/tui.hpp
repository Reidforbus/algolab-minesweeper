#include <cstdint>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>


namespace algolab{
    class GameTUI {
        private:
            uint32_t board_w;
            uint32_t board_h;
            ftxui::Element document;
            ftxui::Element board_grid;
            std::vector<ftxui::Elements> board;

            void build_doc(){
                std::vector<ftxui::Element> grid_vector;


                for (auto j = 0; j < board_h; j++){
                    std::vector<ftxui::Element> row_vector;

                    for (auto i = 0; i < board_w; i++){
                        row_vector.push_back(board[j][i]);
                        if (i != board_w - 1){
                            row_vector.push_back(ftxui::separator());
                        }
                    }

                    grid_vector.push_back(ftxui::hbox(row_vector));
                    if (j != board_w - 1){
                        grid_vector.push_back(ftxui::separator());
                    }

                }
                board_grid = ftxui::vbox(grid_vector) | ftxui::flex_grow;

                document = ftxui::hbox({
                        board_grid | ftxui::borderDouble | ftxui::center,
                        }) | ftxui::center;
            }

            auto square(std::string s, bool open){
                auto sq = ftxui::hbox(ftxui::text(s) | 
                        ftxui::center |
                        ftxui::flex_grow) |
                    ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3) |
                    ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1);
                if (!open) {
                    sq |= ftxui::borderHeavy;
                } else {
                    sq |= ftxui::borderEmpty;
                }
                return sq;
            }

        public:

            void new_game(uint32_t h, uint32_t w){
                board_w = w;
                board_h = h;
                board.clear();


                for (int j = 0; j < h; j++) {
                    std::vector<ftxui::Element> row;
                    for (auto i = 0; i < w; i++) {

                        row.push_back(square("  ", false));
                    }
                    board.push_back(row);
                }

            }

            void update_state(std::vector<uint32_t> state){
                for (auto j = 0;j < board_h; j++){
                    for (auto i = 0;i < board_w; i++){
                        auto val = state[(j * board_w) + i];
                        std::string sq = "  ";
                        bool open = val < 10;

                        if (val < 9 & val > 0){
                            sq = std::to_string(val);
                        } else if (val == 9) {
                            sq = "💣";
                        } else if (val == 11) {
                            sq = "🔺";
                        }
                        board[j][i] = square(sq, open);
                    }
                }
            }

            void draw(){
                build_doc();
                auto screen = ftxui::Screen::Create(
                        ftxui::Dimension::Fit(document, true),
                        ftxui::Dimension::Fit(document, true)
                        );

                ftxui::Render(screen, document);
                screen.Print();
            }
    };
} //namespace algolab
