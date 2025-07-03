#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>


namespace algolab{
    class GameTUI {
        int board_w;
        int board_h;
        ftxui::Element document;
        ftxui::Element board_grid;
        std::vector<ftxui::Elements> board;
        std::vector<ftxui::Element> messages;
        ftxui::Screen screen;

        void build_doc();

        auto square(std::string s, bool open, bool latest);

        public:

        GameTUI();

        void new_game(int h, int w);

        void add_message(std::string s);

        void clear_messages();

        void update_state(std::vector<int> state);

        void draw();
    };
} //namespace algolab
