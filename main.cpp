//
// Created by abdel on 30/11/2023.
//
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include "Production.h"
#include "GrammarChecker.h"

int main() {
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"S", {{"A", "a'"}, {"b"}}},
            {"A'", {{"S", "c"}, {"d"}}},
    };

    GrammarChecker grammarChecker(grammar);
    grammarChecker.isLL1Grammar();

    // Print firstSets
    const auto& firstSets = grammarChecker.getFirstSets();
    std::cout << "First Sets:\n";
    for (const auto& entry : firstSets) {
        const std::string& nonTerminal = entry.first;
        const std::set<char>& firstSet = entry.second;

        std::cout << nonTerminal << ": { ";
        for (char symbol : firstSet) {
            std::cout << symbol << ' ';
        }
        std::cout << "}\n";
    }

    // Print followSets
    const auto& followSets = grammarChecker.getFollowSets();
    std::cout << "\nFollow Sets:\n";
    for (const auto& entry : followSets) {
        const std::string& nonTerminal = entry.first;
        const std::set<char>& followSet = entry.second;

        std::cout << nonTerminal << ": { ";
        for (char symbol : followSet) {
            std::cout << symbol << ' ';
        }
        std::cout << "}\n";
    }

    return 0;
}
