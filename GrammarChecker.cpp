#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include "Production.h"

using namespace std;

class GrammarChecker {
public:
    GrammarChecker(const unordered_map<string, vector<vector<string>>>& grammar);

    bool isLL1Grammar();

private:
    vector<Production> productionVector;
    unordered_map<string, set<char>> computedFirstSets;
    unordered_map<string, set<char>> computedFollowSets;

    set<char> computeFirst(char nonTerminal);
    set<char> computeFollow(char nonTerminal);

    bool hasCommonElements(const unordered_map<string, set<char>>& sets);
    bool hasCommonIntersection();

};

GrammarChecker::GrammarChecker(const unordered_map<string, vector<vector<string>>>& grammar) {
    for (const auto& entry : grammar) {
        const string& nonTerminal = entry.first;
        const vector<vector<string>>& productions = entry.second;

        productionVector.push_back({nonTerminal, productions});
    }
}

set<char> GrammarChecker::computeFirst(char nonTerminal) {
    set<char> firstSet;

    for (const Production& rule : productionVector) {
        if (rule.nonTerminal[0] == nonTerminal) {
            for (const auto& production : rule.productions) {
                for (char symbol : production[0]) {
                    if (isupper(symbol)) {
                        if (computedFirstSets.find(string(1, symbol)) != computedFirstSets.end()) {
                            firstSet.insert(computedFirstSets[string(1, symbol)].begin(), computedFirstSets[string(1, symbol)].end());
                        } else {
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

set<char> GrammarChecker::computeFollow(char nonTerminal) {
    set<char> followSet;

    for (const Production& rule : productionVector) {
        for (const auto& production : rule.productions) {
            for (size_t i = 0; i < production.size(); ++i) {
                if (production[i][0] == nonTerminal) {
                    if (i < production.size() - 1) {
                        if (!isupper(production[i + 1][0])) {
                            followSet.insert(production[i + 1][0]);
                        } else {
                            if (computedFollowSets.find(string(1, production[i + 1][0])) != computedFollowSets.end()) {
                                followSet.insert(computedFollowSets[string(1, production[i + 1][0])].begin(), computedFollowSets[string(1, production[i + 1][0])].end());
                            } else {
                                followSet.insert(production[i + 1][0]);
                            }
                        }
                    } else {
                        if (rule.nonTerminal[0] != nonTerminal) {
                            if (computedFollowSets.find(rule.nonTerminal) != computedFollowSets.end()) {
                                followSet.insert(computedFollowSets[rule.nonTerminal].begin(), computedFollowSets[rule.nonTerminal].end());
                            } else {
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

bool GrammarChecker::hasCommonElements(const unordered_map<string, set<char>>& sets) {
    for (const auto& entry : sets) {
        const string& nonTerminal = entry.first;
        const set<char>& currentSet = entry.second;

        set<char> commonSet;
        for (const Production& rule : productionVector) {
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
    for (const auto& entry : computedFirstSets) {
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
    unordered_map<string, set<char>> firstSets;
    unordered_map<string, set<char>> followSets;
    set<string> nonTerminalsToComputeFirst;
    set<string> nonTerminalsToComputeFollow;

    for (const Production& rule : productionVector) {
        nonTerminalsToComputeFirst.insert(rule.nonTerminal);
        nonTerminalsToComputeFollow.insert(rule.nonTerminal);
    }

    while (!nonTerminalsToComputeFirst.empty()) {
        string currentNonTerminal = *nonTerminalsToComputeFirst.begin();
        nonTerminalsToComputeFirst.erase(nonTerminalsToComputeFirst.begin());
        firstSets[currentNonTerminal] = computeFirst(currentNonTerminal[0]);
    }

    while (!nonTerminalsToComputeFollow.empty()) {
        string currentNonTerminal = *nonTerminalsToComputeFollow.begin();
        nonTerminalsToComputeFollow.erase(nonTerminalsToComputeFollow.begin());
        followSets[currentNonTerminal] = computeFollow(currentNonTerminal[0]);
    }

    if (!hasCommonElements(firstSets)) {
        return false;
    }

    if (!hasCommonIntersection()) {
        return false;
    }

    return true;
}

int main() {
    unordered_map<string, vector<vector<string>>> grammar = {
            {"S", {{"A", "a'"}, {"b"}}},
            {"A'", {{"S", "c"}, {"d"}}},
    };

    GrammarChecker grammarChecker(grammar);
    grammarChecker.isLL1Grammar();

    return 0;
}
