add_executable(Minesweeper main.cpp)

target_include_directories(Minesweeper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Minesweeper
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
