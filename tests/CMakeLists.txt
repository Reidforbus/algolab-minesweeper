add_executable(tests test.cpp)

target_link_libraries(tests 
    PRIVATE tools
    PRIVATE Catch2::Catch2WithMain )

include(CTest)
include(Catch)
catch_discover_tests(tests)
