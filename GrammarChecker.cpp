//
// Created by abdel on 30/11/2023.
//

#include "GrammarChecker.h"
#include "Production.h"

GrammarChecker::GrammarChecker(const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar) {
for (const auto& entry : grammar) {
const std::string& nonTerminal = entry.first;
const std::vector<std::vector<std::string>>& productions = entry.second;

productionVector.push_back({nonTerminal, productions});
}
}

std::unordered_map<std::string, std::set<char>> GrammarChecker::computeFirst(char nonTerminal) {
    set<char> firstSet;

    for (const Production& rule : grammar) {
        if (rule.nonTerminal[0] == nonTerminal) {
            for (const auto& production : rule.productions) {
                for (char symbol : production[0]) {
                    if (isupper(symbol)) {
                        if (computedFirstSets.find(string(1, symbol)) != computedFirstSets.end()) {
                            firstSet.insert(computedFirstSets[string(1, symbol)].begin(), computedFirstSets[string(1, symbol)].end());
                        } else {
                            // Symbol not yet computed, add it to the set of non-terminals to compute
                            firstSet.insert(symbol);
                        }
                    } else {
                        firstSet.insert(symbol);
                        break;
                    }
                }
            }
        }
    }

    return firstSet;
}

std::set<char> GrammarChecker::computeFollow(char nonTerminal) {
    // Implementation of computeFollow
    // ...
}

bool GrammarChecker::hasCommonElements(const std::unordered_map<std::string, std::set<char>>& sets) {
    // Implementation of hasCommonElements
    // ...
}

bool GrammarChecker::hasCommonIntersection() {
    // Implementation of hasCommonIntersection
    // ...
}

bool GrammarChecker::isLL1Grammar() {
    // Implementation of isLL1Grammar
    // ...
}
