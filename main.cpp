//
// Created by abdel on 30/11/2023.
//
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include "GrammarChecker.h"
#include "constants.h"
int main() {
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"S", {{"a", "B", "D", "Y","h"}}},
            {"B", {{"c", "C"}}},
            {"C", {{"b", "C"}, {EPSILON}}},
            {"D", {{"E", "F"}}},
            {"E", {{"g"}, {EPSILON}}},
            {"Y", {{"y"}, {EPSILON}}},
            {"F", {{"f"}, {EPSILON}}}
    };

    GrammarChecker grammarChecker(grammar);
    grammarChecker.isLL1Grammar();
    // Print firstSets
    const auto& firstSets = grammarChecker.getFirstSets();
    cout << "First Sets:\n";
    for (const auto& entry : firstSets) {
        const string& nonTerminal = entry.first;
        const set<char>& firstSet = entry.second;

        cout << nonTerminal << ": { ";
        for (char symbol : firstSet) {
            cout << symbol << ' ';
        }
        cout << "}\n";
    }

    // Print followSets
    const auto& followSets = grammarChecker.getFollowSets();
    cout << "\nFollow Sets:\n";
    for (const auto& entry : followSets) {
        const string& nonTerminal = entry.first;
        const set<char>& followSet = entry.second;

        cout << nonTerminal << ": { ";
        for (char symbol : followSet) {
            cout << symbol << ' ';
        }
        cout << "}\n";
    }
    return 0;
}
