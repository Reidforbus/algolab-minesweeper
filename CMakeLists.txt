cmake_minimum_required(VERSION 3.22)

project(minesweeper)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

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

option(ENABLE_COVERAGE "Enable coverage reporting" ON)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Building with coverage flags")
    add_compile_options(--coverage -O0 -g -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()


add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E echo "Generating coverage"
    COMMAND find . -name '*.gcda' -delete
    COMMAND rm -fr coverage
    COMMAND mkdir coverage
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMAND gcovr . --exclude '/usr/*' --exclude '.*catch2.*' --exclude '.*ftxui.*' --exclude 'src/main.cpp' --exclude 'tests/*' --exclude-throw-branches --exclude-unreachable-branches --html --html-details -o coverage/coverage.html

)

add_executable(minesweeper src/main.cpp)

target_include_directories(minesweeper PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(minesweeper
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)

enable_testing()
add_subdirectory(tests)
