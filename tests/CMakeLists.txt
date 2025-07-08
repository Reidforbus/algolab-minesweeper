set(TEST_SOURCES
    test.cpp
    test_utilities.cpp
    test_game.cpp
    test_aicore.cpp
    test_csp.cpp
    test_algorithm.cpp
)

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests 
    PRIVATE tools
    PRIVATE Catch2::Catch2WithMain )

include(CTest)
include(Catch)
catch_discover_tests(tests)
