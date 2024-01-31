#include <gtest/gtest.h>
#include "Analyzer.h"
#include "Lex.h"

class AnalyzerAndLexFixture : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};


TEST_F(AnalyzerAndLexFixture, AnalyzerAndLex_JavaRules_ValidProgram_ReturnsTokens) {
    std::string program_file_name = "../../tests/test_files/program_test_1_1.txt";
    std::string rules_file_name = "../../tests/test_files/rules_test_1.txt";
    Lex lex(rules_file_name, program_file_name);

    std::vector<std::pair<std::string, std::string>> expected_tokens = {
            {"int",    "int"},
            {"id",     "x"},
            {";",      ";"},
            {"id",     "x"},
            {"assign", "="},
            {"num",    "5"},
            {";",      ";"},
            {"if",     "if"},
            {"(",      "("},
            {"id",     "x"},
            {"relop",  ">"},
            {"num",    "2"},
            {")",      ")"},
            {"{",      "{"},
            {"id",     "x"},
            {"assign", "="},
            {"num",    "0"},
            {";",      ";"},
            {"}",      "}"},
            {"else",   "else"},
            {"{",      "{"},
            {"id",     "x"},
            {"assign", "="},
            {"num",    "1"},
            {";",      ";"},
            {"}",      "}"},
    };

    std::vector<std::pair<int, int>> expected_positions = {
            {0, 1},
            {0, 5},
            {0, 5},
            {1, 1},
            {1, 3},
            {1, 5},
            {1, 5},
            {2, 1},
            {2, 4},
            {2, 4},
            {2, 7},
            {2, 9},
            {2, 9},
            {3, 1},
            {4, 2},
            {4, 4},
            {4, 6},
            {4, 6},
            {5, 1},
            {6, 1},
            {7, 1},
            {8, 2},
            {8, 4},
            {8, 6},
            {8, 6},
            {9, 1}
    };

    lex.buildLex();
    Token *token = lex.getNextToken();
    int i = 0;
    while (*(token->getKey()) != "$") {
        ASSERT_EQ(*(token->getKey()), expected_tokens[i].first);
        ASSERT_EQ(*(token->getValue()), expected_tokens[i].second);
        ASSERT_EQ(token->getPosition()->line_number, expected_positions[i].first);
        ASSERT_EQ(token->getPosition()->column_number, expected_positions[i].second);
        i++;
        token = lex.getNextToken();
    }
}

TEST_F(AnalyzerAndLexFixture, AnalyzerAndLex_JavaRules_NonValidProgram_ReturnsTokens) {
    std::string program_file_name = "../../tests/test_files/program_test_1_2.txt";
    std::string rules_file_name = "../../tests/test_files/rules_test_1.txt";
    Lex lex(rules_file_name, program_file_name);

    std::vector<std::pair<std::string, std::string>> expected_tokens = {
            {"num",     "1.5"},
            {"id",      "E"},
            {"addop",   "-"},
            {"num",     "6"},
            {"int",     "int"},
            {"id",      "sum"},
            {",",       ","},
            {"id",      "count"},
            {",",       ","},
            {"id",      "pass"},
            {",",       ","},
            {"id",      "mnt"},
            {";",       ";"},
            {"id",      "whi"},
            {"id",      "le"},
            {"(",       "("},
            {"id",      "pass"},
            {"num",     "10"},
            {")",       ")"},
            {"{",       "{"},
            {"id",      "pass"},
            {"assign",  "="},
            {"id",      "pass"},
            {"addop",   "+"},
            {"num",     "1"},
            {";",       ";"},
            {"}",       "}"},
            {"boolean", "boolean"},
            {"float",   "float"},
            {"int",     "int"},
            {"id",      "intx"},
            {"id",      "dou"},
            {"id",      "whi"},
            {"id",      "le"},
            {"id",      "do"},
            {"id",      "E"},
            {"while",   "while"},
            {"id",      "E"},
            {"num",     "123"},
            {"id",      "E"},
            {"num",     "1.5"},
            {"id",      "E"},
            {"id",      "do"},
            {"num",     "1.5"},
            {"id",      "E"}
    };

    std::vector<std::pair<int, int>> expected_positions = {
            {0,  1},
            {0,  4},
            {0,  5},
            {0,  5},
            {1,  2},
            {1,  6},
            {1,  10},
            {1,  12},
            {1,  18},
            {1,  20},
            {1,  25},
            {1,  27},
            {1,  29},
            {1,  32},
            {2,  1},
            {2,  4},
            {2,  5},
            {3,  1},
            {3,  4},
            {4,  1},
            {5,  1},
            {5,  6},
            {5,  8},
            {5,  13},
            {5,  15},
            {5,  15},
            {6,  1},
            {7,  1},
            {7,  9},
            {7,  15},
            {7,  19},
            {9,  1},
            {10, 1},
            {10, 5},
            {11, 1},
            {12, 4},
            {12, 6},
            {13, 4},
            {13, 6},
            {14, 4},
            {15, 1},
            {15, 5},
            {16, 1},
            {18, 1},
            {18, 4}
    };

    lex.buildLex();
    Token *token = lex.getNextToken();
    int i = 0;
    while (*(token->getKey()) != "$") {
        ASSERT_EQ(*(token->getKey()), expected_tokens[i].first);
        ASSERT_EQ(*(token->getValue()), expected_tokens[i].second);
        //ASSERT_EQ(token->getPosition()->line_number, expected_positions[i].first);
        //ASSERT_EQ(token->getPosition()->column_number, expected_positions[i].second);
        std::cout << "line: " << token->getPosition()->line_number << " column: " << token->getPosition()->column_number
                  << '\n';
        i++;
        token = lex.getNextToken();
    }
}