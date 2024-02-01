#include <gtest/gtest.h>
#include "Lex/Utilities.h"

class UtilitiesFixture : public ::testing::Test {
protected:
    void SetUp() override {
        std::string input = "digit*|digit+ . digits(\\L|E digits)a-z";
        std::set<std::string> substrings = {"digit", "digits"};
        auto substringInfoVec = Utilities::findAllLongestSubstringIndices(&input, &substrings);
    }

    void TearDown() override {
        // Any cleanup code needed after each test
    }
};

TEST_F(UtilitiesFixture, CleanRegex_ValidInput_ReturnsCleanedRule) {
    std::string input = "  digit * | digit  +   .  digits  (\\L  | E digits  )  a  -  z ";
    std::string expected = "digit*|digit+ . digits(\\L|E digits)a-z";
    auto cleanedRule = Utilities::cleanRegex(&input);
    EXPECT_STREQ(cleanedRule->c_str(), expected.c_str());
}

TEST_F(UtilitiesFixture, FindAllLongestSubstringIndices_ValidInput_ReturnsVectorOfSubstringInfo) {
    std::string input = "digit*|digit+ . digits(\\L|E digits)a-z";
    std::set<std::string> substrings = {"digit", "digits"};
    std::vector<SubstringInfo> expected = {{0,  6},
                                           {7,  13},
                                           {16, 22},
                                           {28, 34}};
    auto substringInfoVec = Utilities::findAllLongestSubstringIndices(&input, &substrings);

    ASSERT_EQ(substringInfoVec.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(substringInfoVec[i].start, expected[i].start);
        EXPECT_EQ(substringInfoVec[i].end, expected[i].end);
    }
}

TEST_F(UtilitiesFixture, FixConcatGivenType_ValidInput_ModifiesRulesObject) {
    // Create input data
    std::unordered_map<std::string, std::pair<std::string, int>> expected_regular_rules = {
            {"num", {"digit+|digit+ . digits (\\L|E digits)", 0}}
    };
    Rules rules; // Initialize Rules object
    rules.addRule(RuleType::REGULAR_EXPRESSION,
                  "  digit + | digit  +   .  digits  (\\L  | E digits  )  ", "num");

    std::set<std::string> non_terminal_symbols = {"digit", "digits"};
    RuleType type = RuleType::REGULAR_EXPRESSION;

    // Call the function to be tested
    Utilities::fixConcatGivenType(rules.getRegularExpressionsMap(), &rules, &non_terminal_symbols, type);

    // Assert that the rules object is modified as expected
    // Add assertions here based on the expected behavior of the function
    for (const auto &entry: expected_regular_rules) {
        ASSERT_TRUE(rules.getRegularExpressionsMap().count(entry.first) == 1);
        EXPECT_STREQ(rules.getRegularExpressionsMap().at(entry.first).first.c_str(), entry.second.first.c_str());
        EXPECT_EQ(entry.second.second, rules.getRegularExpressionsMap().at(entry.first).second);
    }

    ASSERT_EQ(rules.getRegularExpressionsMap().size(), 1);
}

TEST_F(UtilitiesFixture, DetectConcatThenAddSpaces_ValidInput_ReturnsNewOffset) {
    std::string expected_expression = "digit+|digit+ . digits(\\L|E digits)";
    std::string expression = "digit+|digit+.digits(\\L|Edigits)";
    std::vector<SubstringInfo> substringInfoVec = {{0,  6},
                                                   {7,  13},
                                                   {14, 20},
                                                   {25, 31}};
    int expected_offsets[] = {0, 1, 2, 3};
    int actual = 0;
    for (int i = 0; i < substringInfoVec.size(); ++i) {
        actual = Utilities::detectConcatThenAddSpaces(&expression, substringInfoVec, actual, i);
        EXPECT_EQ(actual, expected_offsets[i]);
    }

    EXPECT_STREQ(expression.c_str(), expected_expression.c_str());
}

TEST_F(UtilitiesFixture, AddSpaceAfterAndBeforeBraces_ValidInput_ReturnsModifiedExpression) {
    std::string expected_expression = "(xyz)|(xyz)*|(digit+|digit)+ .digits (\\L|E digits)* ab*";
    std::string expression = "(xyz)|(xyz)*|(digit+|digit)+ .digits (\\L|E digits)*ab*";

    Utilities::addSpaceAfterAndBeforeBraces(&expression);

    EXPECT_STREQ(expression.c_str(), expected_expression.c_str());
}

TEST_F(UtilitiesFixture, FixConcat_ValidInput_ModifiesRulesObject) {
    // Create input data
    std::unordered_map<std::string, std::pair<std::string, int>> expected_regular_rules = {
            {"num", {"digit+|digit+ . digits (\\L|E digits)", 0}}
    };
    Rules rules; // Initialize Rules object
    rules.addRule(RuleType::REGULAR_EXPRESSION,
                  "  digit + | digit  +   .  digits  (\\L  | E digits  )  ", "num");

    std::set<std::string> non_terminal_symbols = {"digit", "digits"};

    // Call the function to be tested
    Utilities::fixConcat(&rules, &non_terminal_symbols);

    // Assert that the rules object is modified as expected
    // Add assertions here based on the expected behavior of the function
    for (const auto &entry: expected_regular_rules) {
        ASSERT_TRUE(rules.getRegularExpressionsMap().count(entry.first) == 1);
        EXPECT_STREQ(rules.getRegularExpressionsMap().at(entry.first).first.c_str(), entry.second.first.c_str());
        EXPECT_EQ(entry.second.second, rules.getRegularExpressionsMap().at(entry.first).second);
    }

    ASSERT_EQ(rules.getRegularExpressionsMap().size(), 1);
}