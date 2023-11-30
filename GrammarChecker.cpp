#include "Production.h"
#include "GrammarChecker.h"

GrammarChecker::GrammarChecker(const unordered_map<string, vector<vector<string>>>& grammar) {
    for (const auto& entry : grammar) {
        const string& nonTerminal = entry.first;
        const vector<vector<string>>& productions = entry.second;

        productionVector.push_back({nonTerminal, productions});
    }
}

set<char> GrammarChecker::computeFirst(string nonTerminal) {
    set<char> firstSet;

    for (const Production& rule : productionVector) {
        if (rule.nonTerminal == nonTerminal) {
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

bool GrammarChecker::hasCommonElements(const unordered_map<string, set<char>>& sets, const vector<Production>& grammar) {
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
                commonSet.clear();
            }
        }
    }

    return true;
}

// Function to check if there are common elements in the First and Follow sets for the same non-terminal
bool GrammarChecker::hasCommonIntersection(const unordered_map<string, set<char>>& firstSets,
                                           const unordered_map<string, set<char>>& followSets) {
    for (const auto& entry : firstSets) {
        const string& nonTerminal = entry.first;
        const set<char>& firstSet = entry.second;
        const set<char>& followSet = followSets.at(nonTerminal);

        set<char> intersection;
        set_intersection(firstSet.begin(), firstSet.end(), followSet.begin(), followSet.end(),
                         inserter(intersection, intersection.begin()));

        if (!intersection.empty()) {
            cout << "Not an LL(1) grammar." << endl;
            return false;
        }
    }

    return true;
}
set<string>  GrammarChecker::collectNonTerminals(const vector<Production>& grammar) {
    set<string> nonTerminals;
    for (const Production& rule : grammar) {
        nonTerminals.insert(rule.nonTerminal);
    }
    return nonTerminals;
}
// Function to compute First sets for each non-terminal
void  GrammarChecker::computeFirstSets(unordered_map<string, set<char>>& firstSets) {
    for (const Production& rule : productionVector ) {
        firstSets[rule.nonTerminal] = computeFirst(rule.nonTerminal);
    }
}

// Function to compute Follow sets for each non-terminal
void  GrammarChecker::computeFollowSets(unordered_map<string, set<char>>& followSets) {
    for (const Production& rule : productionVector) {
        followSets[rule.nonTerminal] = computeFollow(rule.nonTerminal[0]);
    }
}
bool GrammarChecker::isLL1Grammar() {

    computeFirstSets(computedFirstSets);

    // Compute Follow sets for each non-terminal
    computeFollowSets(computedFollowSets);

    set<string> nonTerminals = collectNonTerminals(productionVector);
    // Check if there are any common elements in the First sets of different rules for the same non-terminal
    if (!hasCommonElements(computedFirstSets, productionVector)) {
        return false;
    }

    // Check if there are any common elements in the First and Follow sets for the same non-terminal
    if (!hasCommonIntersection(computedFirstSets, computedFollowSets)) {
        return false;
    }

    cout << "The grammar is LL(1)." << endl;
    return true;
}

int main() {
    unordered_map<string, vector<vector<string>>> grammar = {
            {"E", {{"T", "E'"}}},
            {"E'", {{"+", "T", "E'"}, {""}}},
            {"T", {{"F", "T'"}}},
            {"T'", {{"*", "F", "T'"}, {""}}},
            {"F", {{"(", "E", ")"}, {"id"}}},
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
