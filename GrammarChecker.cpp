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
    set<char> followSet;

    for (const Production& rule : grammar) {
        for (const auto& production : rule.productions) {
            for (size_t i = 0; i < production.size(); ++i) {
                if (production[i][0] == nonTerminal) {
                    if (i < production.size() - 1) {
                        // If the non-terminal is followed by a terminal, add it to the Follow set
                        if (!isupper(production[i + 1][0])) {
                            followSet.insert(production[i + 1][0]);
                        } else {
                            if (computedFollowSets.find(string(1, production[i + 1][0])) != computedFollowSets.end()) {
                                followSet.insert(computedFollowSets[string(1, production[i + 1][0])].begin(), computedFollowSets[string(1, production[i + 1][0])].end());
                            } else {
                                // Symbol not yet computed, add it to the set of non-terminals to compute
                                followSet.insert(production[i + 1][0]);
                            }
                        }
                    } else {
                        // If the non-terminal is at the end of the production, add the Follow set of the left-hand side to the Follow set
                        if (rule.nonTerminal[0] != nonTerminal) {
                            if (computedFollowSets.find(rule.nonTerminal) != computedFollowSets.end()) {
                                followSet.insert(computedFollowSets[rule.nonTerminal].begin(), computedFollowSets[rule.nonTerminal].end());
                            } else {
                                // Symbol not yet computed, add it to the set of non-terminals to compute
                                followSet.insert(rule.nonTerminal[0]);
                            }
                        }
                    }
                }
            }
        }
    }

    return followSet;
}

bool GrammarChecker::hasCommonElements(const std::unordered_map<std::string, std::set<char>>& sets) {
    for (const auto& entry : sets) {
        const string& nonTerminal = entry.first;
        const set<char>& currentSet = entry.second;

        set<char> commonSet;
        for (const Production& rule : grammar) {
            if (rule.nonTerminal == nonTerminal) {
                for (const auto& production : rule.productions) {
                    for (char symbol : currentSet) {
                        if (commonSet.count(symbol) > 0) {
                            cout << "Not an LL(1) grammar." << endl;
                            return false;
                        }
                    }
                    commonSet.insert(currentSet.begin(), currentSet.end());
                }
            }
        }
    }

    return true;
}

bool GrammarChecker::hasCommonIntersection() {
    for (const auto& entry : firstSets) {
        const string& nonTerminal = entry.first;
        const set<char>& intersection = entry.second;

        if (!intersection.empty()) {
            cout << "Not an LL(1) grammar." << endl;
            return false;
        }
    }

    cout << "The grammar is LL(1)." << endl;
    return true;
}

bool GrammarChecker::isLL1Grammar() {
    // Map to store First sets for each non-terminal
    unordered_map<string, set<char>> firstSets;

    // Map to store Follow sets for each non-terminal
    unordered_map<string, set<char>> followSets;

    // Set to keep track of non-terminals for which First sets need to be computed
    set<string> nonTerminalsToComputeFirst;

    // Set to keep track of non-terminals for which Follow sets need to be computed
    set<string> nonTerminalsToComputeFollow;

    // Initialize the sets
    for (const Production& rule : grammar) {
        nonTerminalsToComputeFirst.insert(rule.nonTerminal);
        nonTerminalsToComputeFollow.insert(rule.nonTerminal);
    }

    // Compute First sets for each non-terminal
    while (!nonTerminalsToComputeFirst.empty()) {
        string currentNonTerminal = *nonTerminalsToComputeFirst.begin();
        nonTerminalsToComputeFirst.erase(nonTerminalsToComputeFirst.begin());
        firstSets[currentNonTerminal] = computeFirst(currentNonTerminal[0], grammar, firstSets);
    }

    // Compute Follow sets for each non-terminal
    while (!nonTerminalsToComputeFollow.empty()) {
        string currentNonTerminal = *nonTerminalsToComputeFollow.begin();
        nonTerminalsToComputeFollow.erase(nonTerminalsToComputeFollow.begin());
        followSets[currentNonTerminal] = computeFollow(currentNonTerminal[0], grammar, followSets);
    }

    // Check if there are any common elements in the First sets of different rules for the same non-terminal
    if (!hasCommonElements(firstSets, grammar)) {
        return false;
    }

    // Check if there are any common elements in the First and Follow sets for the same non-terminal
    if (!hasCommonIntersection(firstSets)) {
        return false;
    }

    return true;
}
