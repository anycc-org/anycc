#include <gtest/gtest.h>
#include "FirstAndFollowGenerator.h"
#include "CFGReader.h"

class FirstAndFollowGeneratorFixture : public ::testing::Test {
protected:

    void SetUp() override {
    }

    void TearDown() override {
    }

    static void AssertFirstAndFollowSets(
            const std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> &computedFirstSets,
            const std::unordered_map<std::string, std::set<std::string>> &computedFollowSets,
            const std::unordered_map<std::string, std::set<std::string>> &expectedFirstSets,
            const std::unordered_map<std::string, std::set<std::string>> &expectedFollowSets) {
        // Compare computed first and follow sets with expected sets
        for (const auto &entry: expectedFirstSets) {
            const std::string &nonTerminal = entry.first;
            const std::set<std::string> &expectedFirstSet = entry.second;

            ASSERT_EQ(computedFirstSets.count(nonTerminal), 1);

            const auto &computedFirstSet = computedFirstSets.at(nonTerminal);
            std::set<std::string> computedFirstSymbols;
            for (const auto &pair: computedFirstSet) {
                computedFirstSymbols.insert(pair.first);
            }

            ASSERT_EQ(computedFirstSymbols, expectedFirstSet);
        }

        for (const auto &entry: expectedFollowSets) {
            const std::string &nonTerminal = entry.first;
            const std::set<std::string> &expectedFollowSet = entry.second;

            ASSERT_EQ(computedFollowSets.count(nonTerminal), 1);

            const auto &computedFollowSet = computedFollowSets.at(nonTerminal);
            ASSERT_EQ(computedFollowSet, expectedFollowSet);
        }
    }
};


TEST_F(FirstAndFollowGeneratorFixture, ComputeFirstAndFollowSets_ValidGrammar_JavaCFG_CheckResults) {
    // Define the grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
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

    // Create a FirstAndFollowGenerator instance
    CFGReader::start_symbol = "METHOD_BODY";
    FirstAndFollowGenerator generator(grammar);

    // Compute first and follow sets
    generator.compute();

    // Get the computed first and follow sets
    const auto &computedFirstSets = generator.getFirstSets();
    const auto &computedFollowSets = generator.getFollowSets();

    // Define expected first and follow sets
    std::unordered_map<std::string, std::set<std::string>> expectedFirstSets = {
            {"METHOD_BODY",         {"float",   "id",    "if", "int", "while"}},
            {"TERM",                {"(",       "id",    "num"}},
            {"TERM\"",              {"Epsilon", "mulop"}},
            {"STATEMENT_LIST\"",    {"Epsilon", "float", "id", "if",  "int", "while"}},
            {"ASSIGNMENT",          {"id"}},
            {"WHILE",               {"while"}},
            {"PRIMITIVE_TYPE",      {"float",   "int"}},
            {"DECLARATION",         {"float",   "int"}},
            {"SIGN",                {"addop"}},
            {"EXPRESSION",          {"(",       "addop", "id", "num"}},
            {"EXPRESSION\"",        {"Epsilon", "relop"}},
            {"IF",                  {"if"}},
            {"FACTOR",              {"(",       "id",    "num"}},
            {"SIMPLE_EXPRESSION",   {"(",       "addop", "id", "num"}},
            {"SIMPLE_EXPRESSION\"", {"Epsilon", "addop"}},
            {"STATEMENT",           {"float",   "id",    "if", "int", "while"}},
            {"STATEMENT_LIST",      {"float",   "id",    "if", "int", "while"}}
    };

    std::unordered_map<std::string, std::set<std::string>> expectedFollowSets = {
            {"METHOD_BODY",         {"$"}},
            {"ASSIGNMENT",          {"$", "float", "id",    "if",    "int", "while", "}"}},
            {"STATEMENT",           {"$", "float", "id",    "if",    "int", "while", "}"}},
            {"STATEMENT_LIST",      {"$"}},
            {"STATEMENT_LIST\"",    {"$"}},
            {"DECLARATION",         {"$", "float", "id",    "if",    "int", "while", "}"}},
            {"EXPRESSION",          {")", ";"}},
            {"EXPRESSION\"",        {")", ";"}},
            {"SIMPLE_EXPRESSION",   {")", ";",     "relop"}},
            {"SIMPLE_EXPRESSION\"", {")", ";",     "relop"}},
            {"TERM",                {")", ";",     "addop", "relop"}},
            {"TERM\"",              {")", ";",     "addop", "relop"}},
            {"FACTOR",              {")", ";",     "addop", "mulop", "relop"}},
            {"SIGN",                {"(", "id",    "num"}},
            {"PRIMITIVE_TYPE",      {"id"}},
            {"IF",                  {"$", "float", "id",    "if",    "int", "while", "}"}},
            {"WHILE",               {"$", "float", "id",    "if",    "int", "while", "}"}}
    };

    AssertFirstAndFollowSets(computedFirstSets, computedFollowSets, expectedFirstSets, expectedFollowSets);
}

TEST_F(FirstAndFollowGeneratorFixture, ComputeFirstAndFollowSets_CFG2_CheckResults) {
    // Define the grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"S", {{"A", "b", "S"}, {"e"}, {"Epsilon"}}},
            {"A", {{"a"},           {"c", "A", "d"}}}
    };

    // Create a FirstAndFollowGenerator instance
    CFGReader::start_symbol = "S";
    FirstAndFollowGenerator generator(grammar);

    // Compute first and follow sets
    generator.compute();

    // Get the computed first and follow sets
    const auto &computedFirstSets = generator.getFirstSets();
    const auto &computedFollowSets = generator.getFollowSets();

    // Define expected first and follow sets
    std::unordered_map<std::string, std::set<std::string>> expectedFirstSets = {
            {"S", {"Epsilon", "a", "c", "e"}},
            {"A", {"a",       "c"}}
    };

    std::unordered_map<std::string, std::set<std::string>> expectedFollowSets = {
            {"A", {"b", "d"}},
            {"S", {"$"}}
    };

    AssertFirstAndFollowSets(computedFirstSets, computedFollowSets, expectedFirstSets, expectedFollowSets);
}

TEST_F(FirstAndFollowGeneratorFixture, ComputeFirstAndFollowSets_CFG3_CheckResults) {
    // Define the grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"S", {{"R", "T"}}},
            {"R", {{"s", "U", "R", "b"}, {"Epsilon"}}},
            {"U", {{"u", "U"},           {"Epsilon"}}},
            {"V", {{"v", "V"},           {"Epsilon"}}},
            {"T", {{"V", "t", "T"},      {"Epsilon"}}}
    };

    // Create a FirstAndFollowGenerator instance
    CFGReader::start_symbol = "S";
    FirstAndFollowGenerator generator(grammar);

    // Compute first and follow sets
    generator.compute();

    // Get the computed first and follow sets
    const auto &computedFirstSets = generator.getFirstSets();
    const auto &computedFollowSets = generator.getFollowSets();

    // Define expected first and follow sets
    std::unordered_map<std::string, std::set<std::string>> expectedFirstSets = {
            {"V", {"Epsilon", "v"}},
            {"U", {"Epsilon", "u"}},
            {"T", {"Epsilon", "t", "v"}},
            {"S", {"Epsilon", "s", "t", "v"}},
            {"R", {"Epsilon", "s"}}
    };

    std::unordered_map<std::string, std::set<std::string>> expectedFollowSets = {
            {"V", {"t"}},
            {"U", {"b", "s"}},
            {"T", {"$"}},
            {"R", {"$", "b", "t", "v"}},
            {"S", {"$"}}
    };

    AssertFirstAndFollowSets(computedFirstSets, computedFollowSets, expectedFirstSets, expectedFollowSets);
}

TEST_F(FirstAndFollowGeneratorFixture, ComputeFirstAndFollowSets_CustomGrammar_CFG4_CheckResults) {
    // Define the grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"E",  {{"T",   "E1"}}},
            {"E1", {{"add", "T", "E1"}, {"Epsilon"}}},
            {"T",  {{"F",   "T1"}}},
            {"T1", {{"mul", "F", "T1"}, {"Epsilon"}}},
            {"F",  {{"(",   "E", ")"},  {"id"}}}
    };

    // Create a FirstAndFollowGenerator instance
    CFGReader::start_symbol = "E";
    FirstAndFollowGenerator generator(grammar);

    // Compute first and follow sets
    generator.compute();

    // Get the computed first and follow sets
    const auto &computedFirstSets = generator.getFirstSets();
    const auto &computedFollowSets = generator.getFollowSets();

    // Define expected first and follow sets
    std::unordered_map<std::string, std::set<std::string>> expectedFirstSets = {
            {"T",  {"(",       "id"}},
            {"F",  {"(",       "id"}},
            {"T1", {"Epsilon", "mul"}},
            {"E1", {"Epsilon", "add"}},
            {"E",  {"(",       "id"}}
    };

    std::unordered_map<std::string, std::set<std::string>> expectedFollowSets = {
            {"T",  {"$", ")", "add"}},
            {"F",  {"$", ")", "add", "mul"}},
            {"T1", {"$", ")", "add"}},
            {"E1", {"$", ")"}},
            {"E",  {"$", ")"}}
    };

    AssertFirstAndFollowSets(computedFirstSets, computedFollowSets, expectedFirstSets, expectedFollowSets);
}
