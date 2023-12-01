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
    unordered_map<string, vector<vector<string>>> grammar = {
            {"S", {{"A", "C", "B"}, {"C", "b", "b"}, {"B", "a"}}},
            {"A", {{"d", "a"}, {"B", "C"}}},
            {"B", {{"g"}, {EPSILON}}},
            {"C", {{"h"}, {EPSILON}}}
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
