#include <iostream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int main(int argc, char** argv) {
    std::cout << "Hello World" << std::endl;
    ftxui::Element document =
        ftxui::hbox({
                ftxui::text("left") | ftxui::border,
                ftxui::text("center") | ftxui::border,
                ftxui::text("right") | ftxui::border,
                });
    auto screen = ftxui::Screen::Create(
            ftxui::Dimension::Full(),
            ftxui::Dimension::Fit(document)
            );
    ftxui::Render(screen, document);
    screen.Print();

    return EXIT_SUCCESS;
}
