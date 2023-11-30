//
// Created by abdel on 30/11/2023.
//
#include "GrammarChecker.h"

int main() {
    // Modified example grammar with unique non-terminals
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"S", {{"A", "a'"}, {"b"}}},
            {"A'", {{"S", "c"}, {"d"}}},
    };

    // Create an instance of GrammarChecker
    GrammarChecker grammarChecker(grammar);

    // Check if the grammar is LL(1)
    grammarChecker.isLL1Grammar();

    return 0;
}
