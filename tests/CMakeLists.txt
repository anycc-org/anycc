cmake_minimum_required(VERSION 3.27)
project(tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        tests
        Lex/UtilitiesTests.cpp
        Lex/InputReaderTests.cpp
        Lex/NFAGeneratorTests.cpp
        Lex/NFAStateTests.cpp
        Lex/NFATests.cpp
        Lex/TransitionDiagramTests.cpp
        Lex/DeterministicTransitionDiagramAndMinimizerTest.cpp
        Lex/AnalyzerAndLexTests.cpp

        Parser/CFGReaderTests.cpp
        Parser/FirstAndFollowGeneratorTests.cpp
        Parser/ConvertToLL1Tests.cpp
        Parser/PredictiveTableTests.cpp
        AnyccTests.cpp
)

target_link_libraries(
        tests
        anycc_lib
)

target_link_libraries(
        tests
        GTest::gtest_main
        GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(tests)