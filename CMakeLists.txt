cmake_minimum_required(VERSION 3.22)

project(Minesweeper)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
    GIT_TAG v6.1.9
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    EXCLUDE_ALL
)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(ftxui)
FetchContent_MakeAvailable(Catch2)

add_executable(Minesweeper src/main.cpp)

add_executable(tests src/tests/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

target_include_directories(Minesweeper PRIVATE src)

target_link_libraries(Minesweeper
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
