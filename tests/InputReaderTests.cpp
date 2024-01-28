#include <gtest/gtest.h>
#include "Lex/InputReader.h"

class InputReaderFixture : public ::testing::Test {
protected:
    InputReader *inputReader{};
    Rules *rules{};

    void SetUp() override {
        rules = new Rules();
    }

    void TearDown() override {
        delete inputReader;
        delete rules;
    }
};

TEST_F(InputReaderFixture, inputReaderFixture_validInput_readRules) {
    std::string rules_file_name = "../../tests/test_files/rules_test_1.txt";
    inputReader = new InputReader(rules_file_name, rules);

    std::unordered_map<std::string, int> expected_token_priorities = {
            {"}",       1},
            {"{",       1},
            {"(",       1},
            {")",       1},
            {";",       1},
            {",",       1},
            {"letter",  2},
            {"id",      4},
            {"digit",   3},
            {"else",    0},
            {"digits",  5},
            {"mulop",   10},
            {"num",     6},
            {"relop",   7},
            {"int",     0},
            {"float",   0},
            {"assign",  8},
            {"addop",   9},
            {"boolean", 0},
            {"if",      0},
            {"while",   0}
    };

    std::unordered_map<std::string, std::pair<std::string, int>> expected_regular_expressions_map = {
            {"num",    {"digit+|digit+ . digits (\\L|E digits)", 1}},
            {"id",     {"letter (letter|digit)*",                0}},
            {"relop",  {"\\=\\=|!\\=|>|>\\=|<|<\\=",             2}},
            {"addop",  {"\\+|\\-",                               4}},
            {"mulop",  {"\\*|/",                                 5}},
            {"assign", {"\\=",                                   3}}
    };

    std::unordered_map<std::string, std::pair<std::string, int>> expected_regular_definitions_map = {
            {"letter", {"a-z|A-Z", 0}},
            {"digit",  {"0-9",     1}},
            {"digits", {"digit+",  2}}
    };

    std::vector<std::string> expected_keywords = {"boolean", "int", "float", "while", "if", "else"};

    std::vector<std::string> expected_punctuations = {";", ",", "(", ")", "{", "}"};

    // Assert that the rules object is modified as expected
    ASSERT_TRUE(rules != nullptr);
    // Assert sizes
    ASSERT_EQ(rules->getTokensPriority().size(), expected_token_priorities.size());
    ASSERT_EQ(rules->getRegularExpressionsMap().size(), expected_regular_expressions_map.size());
    ASSERT_EQ(rules->getRegularDefinitionsMap().size(), expected_regular_definitions_map.size());
    ASSERT_EQ(rules->getKeywords().size(), expected_keywords.size());
    ASSERT_EQ(rules->getPunctuations().size(), expected_punctuations.size());

    // Loop over the expected token priorities and assert that the actual token priorities are equal to the expected
    for (auto &expected_token_priority: expected_token_priorities) {
        ASSERT_EQ(rules->getTokensPriority()[expected_token_priority.first], expected_token_priority.second);
    }

    // Loop over the expected regular expressions and assert that the actual regular expressions are equal to the expected
    for (auto &expected_regular_expression: expected_regular_expressions_map) {
        ASSERT_EQ(rules->getRegularExpressionsMap()[expected_regular_expression.first].first,
                  expected_regular_expression.second.first);
        ASSERT_EQ(rules->getRegularExpressionsMap()[expected_regular_expression.first].second,
                  expected_regular_expression.second.second);
    }

    // Loop over the expected regular definitions and assert that the actual regular definitions are equal to the expected
    for (auto &expected_regular_definition: expected_regular_definitions_map) {
        ASSERT_EQ(rules->getRegularDefinitionsMap()[expected_regular_definition.first].first,
                  expected_regular_definition.second.first);
        ASSERT_EQ(rules->getRegularDefinitionsMap()[expected_regular_definition.first].second,
                  expected_regular_definition.second.second);
    }

    // Loop over the expected keywords and assert that the actual keywords are equal to the expected
    for (int i = 0; i < expected_keywords.size(); i++) {
        ASSERT_EQ(rules->getKeywords()[i], expected_keywords[i]);
    }

    // Loop over the expected punctuations and assert that the actual punctuations are equal to the expected
    for (int i = 0; i < expected_punctuations.size(); i++) {
        ASSERT_EQ(rules->getPunctuations()[i], expected_punctuations[i]);
    }
}

