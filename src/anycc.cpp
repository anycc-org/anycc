#include <map>
#include "Lex/Lex.h"
#include "Parser/FirstAndFollowGenerator.h"
#include "Parser/LeftRecursionRemover.h"
#include "constants.h"
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include "Lex/Utilities.h"
#include "Parser/PredictiveTable.h"

int main() {
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = Utilities::parseCFGInput(
            "../CFG.txt");
    // for(const auto& kv : grammar) {
    //     std::cout << kv.first << " --> ";
    //     for(const auto& rhs : kv.second) {
    //         for(const auto& prod : rhs) {
    //             std::cout << prod;
    //         }
    //         std::cout << " | ";
    //     }
    //     std::cout << "\n";
    // }
    // auto new_grammar = LeftRecursionRemover::removeImmediateLR(grammar, "S");
    // std::cout << "asd\n";
    // for(const auto& kv : new_grammar) {
    //     std::cout << kv.first << "\n";
    //     std::cout << kv.first << " --> ";
    //     for(const auto& rhs : kv.second) {
    //         for(const auto& prod : rhs) {
    //             std::cout << prod;
    //         }
    //         std::cout << " | ";
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << "asd";
    FirstAndFollowGenerator firstAndFollowGenerator(grammar);
    firstAndFollowGenerator.compute();
    std::cout << "555555555555555\n";
    // Print first_sets
    const auto &first_sets = firstAndFollowGenerator.getFirstSets();
    std::cout << "First Sets:\n";
    for (const auto &entry: first_sets) {
        const std::string &non_terminal = entry.first;
        const std::set<std::pair<std::string, Production>, CompareFirst> &first_set = entry.second;

        std::cout << non_terminal << ": { ";
        for (std::pair<std::string, Production> symbol: first_set) {
            std::cout << symbol.first << ' ';
        }
        std::cout << "}\n";
    }

    // Print followSets
    const auto &followSets = firstAndFollowGenerator.getFollowSets();
    std::cout << "\nFollow Sets:\n";
    for (const auto &entry: followSets) {
        const std::string &non_terminal = entry.first;
        const std::set<std::string> &follow_set = entry.second;

        std::cout << non_terminal << ": { ";
        for (std::string symbol: follow_set) {
            std::cout << symbol << ' ';
        }
        std::cout << "}\n";
    }

    // build predictive table
    PredictiveTable predictiveTable(first_sets, followSets, firstAndFollowGenerator.getNonTerminals());
    predictiveTable.buildPredictiveTable();
    std::cout << "\nPredictive Table:\n";
    predictiveTable.printPredictiveTable();


    return 0;
}
