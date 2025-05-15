cmake_minimum_required(VERSION 3.22)

project(Minesweeper)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
    GIT_TAG v6.1.9
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    EXCLUDE_ALL
)

FetchContent_MakeAvailable(ftxui)

add_executable(Minesweeper src/main.cpp)

target_include_directories(Minesweeper PRIVATE src)

target_link_libraries(Minesweeper
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
