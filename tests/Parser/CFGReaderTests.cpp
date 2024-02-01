#include <gtest/gtest.h>
#include "CFGReader.h"

class CFGReaderFixture : public ::testing::Test {
protected:

    void SetUp() override {
    }

    void TearDown() override {
    }
};


TEST_F(CFGReaderFixture, ParseCFGInput_ValidGrammarFile_JavaCFG_ReturnsCorrectGrammar) {
    std::string cfg_file_name = "../../tests/test_files/cfg_test_1.txt";
    auto grammar = CFGReader::parseCFGInput(cfg_file_name);

    // Check if the grammar is correctly parsed
    ASSERT_EQ(grammar.size(), 13); // Expecting 10 non-terminals

    // Check each non-terminal and its productions
    ASSERT_EQ(grammar["METHOD_BODY"].size(), 1); // Expecting one production for METHOD_BODY
    ASSERT_EQ(grammar["STATEMENT_LIST"].size(), 2); // Expecting two productions for STATEMENT_LIST
    ASSERT_EQ(grammar["STATEMENT"].size(), 4); // Expecting six productions for STATEMENT
    ASSERT_EQ(grammar["DECLARATION"].size(), 1); // Expecting one production for DECLARATION
    ASSERT_EQ(grammar["PRIMITIVE_TYPE"].size(), 2); // Expecting two productions for PRIMITIVE_TYPE
    ASSERT_EQ(grammar["IF"].size(), 1); // Expecting one production for IF
    ASSERT_EQ(grammar["WHILE"].size(), 1); // Expecting one production for WHILE
    ASSERT_EQ(grammar["ASSIGNMENT"].size(), 1); // Expecting one production for ASSIGNMENT
    ASSERT_EQ(grammar["EXPRESSION"].size(), 2); // Expecting two productions for EXPRESSION
    ASSERT_EQ(grammar["SIMPLE_EXPRESSION"].size(), 3); // Expecting three productions for SIMPLE_EXPRESSION
    ASSERT_EQ(grammar["TERM"].size(), 2); // Expecting two productions for TERM
    ASSERT_EQ(grammar["FACTOR"].size(), 3); // Expecting three productions for FACTOR
    ASSERT_EQ(grammar["SIGN"].size(), 1); // Expecting six productions for REL_OP
}

TEST_F(CFGReaderFixture, ParseCFGInput_ValidGrammarFile_CFG2_ReturnsCorrectGrammar) {
    std::string cfg_file_name = "../../tests/test_files/cfg_test_2.txt";
    auto grammar = CFGReader::parseCFGInput(cfg_file_name);

    // Check if the grammar is correctly parsed
    ASSERT_EQ(grammar.size(), 2); // Expecting 10 non-terminals

    // Check each non-terminal and its productions
    ASSERT_EQ(grammar["S"].size(), 3); // Expecting one production for S
    ASSERT_EQ(grammar["A"].size(), 2); // Expecting two productions for A
}

TEST_F(CFGReaderFixture, ParseCFGInput_ValidGrammarFile_CFG3_ReturnsCorrectGrammar) {
    std::string cfg_file_name = "../../tests/test_files/cfg_test_3.txt";
    auto grammar = CFGReader::parseCFGInput(cfg_file_name);

    // Check if the grammar is correctly parsed
    ASSERT_EQ(grammar.size(), 5); // Expecting 10 non-terminals

    // Check each non-terminal and its productions
    ASSERT_EQ(grammar["S"].size(), 1); // Expecting one production for S
    ASSERT_EQ(grammar["R"].size(), 2); // Expecting two productions for R
    ASSERT_EQ(grammar["T"].size(), 2); // Expecting two productions for T
    ASSERT_EQ(grammar["U"].size(), 2); // Expecting two productions for U
    ASSERT_EQ(grammar["V"].size(), 2); // Expecting two productions for V
}

TEST_F(CFGReaderFixture, ParseCFGInput_ValidGrammarFile_CFG4_ReturnsCorrectGrammar) {
    std::string cfg_file_name = "../../tests/test_files/cfg_test_4.txt";
    auto grammar = CFGReader::parseCFGInput(cfg_file_name);

    // Check if the grammar is correctly parsed
    ASSERT_EQ(grammar.size(), 5); // Expecting 10 non-terminals

    // Check each non-terminal and its productions
    ASSERT_EQ(grammar["E"].size(), 1); // Expecting one production for E
    ASSERT_EQ(grammar["E1"].size(), 2); // Expecting two productions for E1
    ASSERT_EQ(grammar["T"].size(), 1); // Expecting two productions for T
    ASSERT_EQ(grammar["T1"].size(), 2); // Expecting two productions for T1
    ASSERT_EQ(grammar["F"].size(), 2); // Expecting two productions for F
}