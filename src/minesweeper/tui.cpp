#include "minesweeper/tui.hpp"
#include <iostream>

namespace algolab {
    GameTUI::GameTUI():
        screen(ftxui::Screen::Create( ftxui::Dimension::Full(), ftxui::Dimension::Full()))
    {}

    void GameTUI::build_doc(){
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
            if (j != board_h - 1){
                grid_vector.push_back(ftxui::separator());
            }

        }
        board_grid = ftxui::vbox(grid_vector) | ftxui::flex_grow;


        document = ftxui::hbox({
                board_grid | ftxui::borderDouble | ftxui::center,
                ftxui::vbox(messages) | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 20),
                }) | ftxui::center;
    }

    auto GameTUI::square(std::string s, bool open, bool latest){
        ftxui::Color number_color;
        ftxui::BorderStyle border;
        ftxui::Color border_color = ftxui::Color::White;



        if (!open) {
            border = ftxui::HEAVY;
        } else {
            border = ftxui::EMPTY;


            switch (std::atoi(s.c_str())){
                case 1:
                    number_color = ftxui::Color::Blue;
                    break;
                case 2:
                    number_color = ftxui::Color::Green;
                    break;
                case 3:
                    number_color = ftxui::Color::Red;
                    break;
                case 4:
                    number_color = ftxui::Color::Magenta;
                    break;
                case 5:
                    number_color = ftxui::Color::Yellow;
                    break;
                case 6:
                    number_color = ftxui::Color::Cyan;
                    break;
                default:
                    break;
            }
        }
        if (latest){
            border = ftxui::LIGHT;
            border_color = ftxui::Color::Red;
        }
        auto content = ftxui::text(s) | 
            ftxui::center |
            ftxui::flex_grow |
            ftxui::color(number_color);

        auto sq = ftxui::hbox(content) |
            ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3) |
            ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1) |
            ftxui::borderStyled(border) |
            ftxui::color(border_color);
        return sq;
    }


    void GameTUI::new_game(int h, int w){
        board_w = w;
        board_h = h;
        board.clear();
        messages.clear();


        for (int j = 0; j < h; j++) {
            std::vector<ftxui::Element> row;
            for (auto i = 0; i < w; i++) {

                row.push_back(square("  ", false, false));
            }
            board.push_back(row);
        }

    }

    void GameTUI::add_message(std::string s){
        messages.push_back(ftxui::text(s));
    }

    void GameTUI::clear_messages(){
        messages.clear();
    }

    void GameTUI::update_state(std::vector<int> state){
        for (auto j = 0;j < board_h; j++){
            for (auto i = 0;i < board_w; i++){
                auto val = state[(j * board_w) + i];
                bool last_move = val >= 16;
                val %= 16;
                std::string sq = "  ";
                bool open = val < 10;

                if (val < 9 && val > 0){
                    sq = std::to_string(val);
                } else if (val == 9) {
                    sq = "💣";
                } else if (val == 11) {
                    sq = "🚩";
                }
                board[j][i] = square(sq, open, last_move);
            }
        }
    }

    void GameTUI::draw(){
        build_doc();

        ftxui::Render(screen, document);
        std::cout << screen.ToString() << std::flush;
    }
}
