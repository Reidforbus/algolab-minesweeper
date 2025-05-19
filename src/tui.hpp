#include <cstdint>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>

class GameTUI {
    private:
        uint32_t board_w;
        uint32_t board_h;
        ftxui::Element document;
        ftxui::Element board_grid;
        std::vector<ftxui::Elements> board;

    public:

        void new_game(uint32_t w, uint32_t h){
            board_w = w;
            board_h = h;
            board.clear();


            for (int j = 0; j < h; j++) {
                std::vector<ftxui::Element> row;
                for (int i = 0; i < w; i++) {

                    row.push_back(ftxui::text("#") |
                            ftxui::border |
                            ftxui::size( ftxui::HEIGHT, ftxui::EQUAL, 6) |
                            ftxui::size( ftxui::WIDTH, ftxui::EQUAL, 6) |
                            ftxui::center
                            );
                }
                board.push_back(row);
            }

            board_grid = ftxui::gridbox(board);
            document = ftxui::hbox({
                    board_grid | ftxui::border,
                    });
        }

        void draw(){
            auto screen = ftxui::Screen::Create(
                    ftxui::Dimension::Fit(document)
                    );

            ftxui::Render(screen, document);
            screen.Print();
        }
};
