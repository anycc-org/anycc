#include <gtest/gtest.h>
#include "FirstAndFollowGenerator.h"
#include "CFGReader.h"
#include "LeftRecursionRemover.h"
#include "LeftFactorer.h"

class ConvertToLL1Fixture : public ::testing::Test {
protected:

    void SetUp() override {
    }

    void TearDown() override {
    }
};


TEST_F(ConvertToLL1Fixture, ComputeFirstAndFollowSets_ValidGrammar_JavaCFG_CheckResults) {
    // Define the grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"METHOD_BODY",       {{"STATEMENT_LIST"}}},
            {"STATEMENT_LIST",    {{"STATEMENT"},         {"STATEMENT_LIST",    "STATEMENT"}}},
            {"STATEMENT",         {{"DECLARATION"},       {"IF"},                        {"WHILE"}, {"ASSIGNMENT"}}},
            {"DECLARATION",       {{"PRIMITIVE_TYPE", "id",     ";"}}},
            {"PRIMITIVE_TYPE",    {{"int"},               {"float"}}},
            {"IF",                {{"if",             "(",      "EXPRESSION", ")", "{", "STATEMENT", "}", "else", "{", "STATEMENT", "}"}}},
            {"WHILE",             {{"while",          "(",      "EXPRESSION", ")", "{", "STATEMENT", "}"}}},
            {"ASSIGNMENT",        {{"id",             "assign", "EXPRESSION", ";"}}},
            {"EXPRESSION",        {{"SIMPLE_EXPRESSION"}, {"SIMPLE_EXPRESSION", "relop", "SIMPLE_EXPRESSION"}}},
            {"SIMPLE_EXPRESSION", {{"TERM"},              {"SIGN",              "TERM"}, {"SIMPLE_EXPRESSION", "addop",      "TERM"}}},
            {"TERM",              {{"FACTOR"},            {"TERM",              "mulop", "FACTOR"}}},
            {"FACTOR",            {{"id"},                {"num"},                       {"(",                 "EXPRESSION", ")"}}},
            {"SIGN",              {{"addop"}}}
    };


    std::unordered_map<std::string, std::vector<std::vector<std::string>>> expected_grammar = {
            {"METHOD_BODY",         {{"STATEMENT_LIST"}}},
            {"EXPRESSION\"",        {{"Epsilon"},                                            {"relop", "SIMPLE_EXPRESSION"}}},
            {"SIMPLE_EXPRESSION\"", {{"addop",             "TERM",   "SIMPLE_EXPRESSION\""}, {"Epsilon"}}},
            {"STATEMENT_LIST\"",    {{"STATEMENT",         "STATEMENT_LIST\""},              {"Epsilon"}}},
            {"TERM\"",              {{"mulop",             "FACTOR", "TERM\""},              {"Epsilon"}}},
            {"STATEMENT_LIST",      {{"STATEMENT",         "STATEMENT_LIST\""}}},
            {"STATEMENT",           {{"DECLARATION"},                                        {"IF"},  {"WHILE"}, {"ASSIGNMENT"}}},
            {"DECLARATION",         {{"PRIMITIVE_TYPE",    "id",     ";"}}},
            {"SIGN",                {{"addop"}}},
            {"ASSIGNMENT",          {{"id",                "assign", "EXPRESSION", ";"}}},
            {"IF",                  {{"if",                "(",      "EXPRESSION", ")", "{", "STATEMENT", "}", "else", "{", "STATEMENT", "}"}}},
            {"EXPRESSION",          {{"SIMPLE_EXPRESSION", "EXPRESSION\""}}},
            {"PRIMITIVE_TYPE",      {{"int"},                                                {"float"}}},
            {"WHILE",               {{"while",             "(",      "EXPRESSION", ")", "{", "STATEMENT", "}"}}},
            {"SIMPLE_EXPRESSION",   {{"TERM",              "SIMPLE_EXPRESSION\""},           {"SIGN",  "TERM", "SIMPLE_EXPRESSION\""}}},
            {"TERM",                {{"FACTOR",            "TERM\""}}},
            {"FACTOR",              {{"id"},                                                 {"num"}, {"(", "EXPRESSION", ")"}}}
    };

    // Call LeftFactorer::leftFactor and LeftRecursionRemover::removeLR on the grammar and check the results
    auto lr_free_grammar = LeftRecursionRemover::removeLR(grammar);
    auto factored_grammar = LeftFactorer::leftFactor(lr_free_grammar);

    // check the expected grammar vs the factored grammar
    ASSERT_EQ(expected_grammar, factored_grammar);
}

TEST_F(ConvertToLL1Fixture, ComputeFirstAndFollowSets_CFG2_NoLRNeitherFactoring_CheckResults) {
    // Define the grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"S", {{"A", "b", "S"}, {"e"}, {"Epsilon"}}},
            {"A", {{"a"},           {"c", "A", "d"}}}
    };

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> expected_grammar = {
            {"S", {{"A", "b", "S"}, {"e"}, {"Epsilon"}}},
            {"A", {{"a"},           {"c", "A", "d"}}}
    };

    // Call LeftFactorer::leftFactor and LeftRecursionRemover::removeLR on the grammar and check the results
    auto lr_free_grammar = LeftRecursionRemover::removeLR(grammar);
    auto factored_grammar = LeftFactorer::leftFactor(lr_free_grammar);

    // check the expected grammar vs the factored grammar
    ASSERT_EQ(expected_grammar, factored_grammar);
}

TEST_F(ConvertToLL1Fixture, ComputeFirstAndFollowSets_CFG5_CheckResults) {
    // Define the grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"E", {{"E", "add", "T"}, {"T"}}},
            {"T", {{"T", "mul", "F"}, {"F"}}},
            {"F", {{"(", "E",   ")"}, {"id"}}}
    };

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> expected_grammar = {
            {"E",   {{"T",   "E\""}}},
            {"E\"", {{"add", "T", "E\""}, {"Epsilon"}}},
            {"T",   {{"F",   "T\""}}},
            {"T\"", {{"mul", "F", "T\""}, {"Epsilon"}}},
            {"F",   {{"(",   "E", ")"},   {"id"}}}
    };

    // Call LeftFactorer::leftFactor and LeftRecursionRemover::removeLR on the grammar and check the results
    auto lr_free_grammar = LeftRecursionRemover::removeLR(grammar);
    auto factored_grammar = LeftFactorer::leftFactor(lr_free_grammar);

    // check the expected grammar vs the factored grammar]
    ASSERT_EQ(expected_grammar, factored_grammar);
}

TEST_F(ConvertToLL1Fixture, ComputeFirstAndFollowSets_CustomGrammar_CFG6_CheckResults) {
    // Define the grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"S", {{"a", "S"},      {"b", "X"}}},
            {"X", {{"X", "X", "c"}, {"X", "d"}, {"Y"}}},
            {"Y", {{"Y", "e"},      {"f"},      {"g"}}}
    };

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> expected_grammar = {
            {"Y",   {{"f", "Y\""},      {"g", "Y\""}}},
            {"X\"", {{"X", "c", "X\""}, {"d", "X\""}, {"Epsilon"}}},
            {"Y\"", {{"e", "Y\""},      {"Epsilon"}}},
            {"X",   {{"Y", "X\""}}},
            {"S",   {{"a", "S"},        {"b", "X"}}},
    };

    // Call LeftFactorer::leftFactor and LeftRecursionRemover::removeLR on the grammar and check the results
    auto lr_free_grammar = LeftRecursionRemover::removeLR(grammar);
    auto factored_grammar = LeftFactorer::leftFactor(lr_free_grammar);

    // check the expected grammar vs the factored grammar
    ASSERT_EQ(expected_grammar, factored_grammar);
}
