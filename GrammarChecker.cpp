#include "Production.h"
#include "GrammarChecker.h"
#include "constants.h"
#include "GrammarCheckerUtility.h"

GrammarChecker::GrammarChecker(const unordered_map<string, vector<vector<string>>>& grammar) {
    for (const auto& entry : grammar) {
        const string& nonTerminal = entry.first;
        const vector<vector<string>>& productions = entry.second;

        productionVector.push_back({nonTerminal, productions});
    }

    nonTerminals = collectNonTerminals(productionVector);
}

set<char> GrammarChecker::computeFirst(const string& nonTerminal) {
    set<char> firstSet;

    for (const Production& rule : productionVector) {
        if (rule.nonTerminal == nonTerminal) {
            for (const auto& production : rule.productions) {
                for (const string& symbol : production) {
                    if (isupper(symbol[0])) {
                        // Handle non-terminal symbols
                        const string& symbolStr = symbol;

                        // Compute First set for the non-terminal
                        const set<char>& nonTerminalFirstSet = computeFirst(symbolStr);
                        firstSet.insert(nonTerminalFirstSet.begin(), nonTerminalFirstSet.end());

                        // Check if the non-terminal has an epsilon production
                        if (!nonTerminalHasEpsilon(symbolStr)) {
                            // If it doesn't have epsilon, break the loop
                            break;
                        }
                    } else {
                        // Handle terminal symbols
                         firstSet.insert(symbol[0]);
                        // Break the loop for terminal symbols
                        break;
                    }
                }
            }
        }
    }

    return firstSet;
}
bool GrammarChecker::nonTerminalHasEpsilon(const string& nonTerminal) {
    // Check if the non-terminal has an epsilon production
    for (const Production& rule : productionVector) {
        if (rule.nonTerminal == nonTerminal) {
            for (const auto& production : rule.productions) {
                if (production.size() == 1 && production[0] == EPSILON) {
                    return true;
                }
            }
        }
    }
    return false;
}
set<char> GrammarChecker::computeFollow(const string& nonTerminal) {
    // If Follow set is already computed, return it
    if (computedFollowSets.find(nonTerminal) != computedFollowSets.end()) {
        return computedFollowSets[nonTerminal];
    }

    set<char> followSet;

    // Mark the Follow set as computed to handle recursive calls
    computedFollowSets[nonTerminal] = followSet;

    // The start symbol has $ (end of input) in its Follow set
    if (nonTerminal == START_SYMBOL) {
        followSet.insert('$');
    }

    for (const Production& rule : productionVector) {
        for (const auto& production : rule.productions) {
            auto pos = find(production.begin(), production.end(), nonTerminal);
            if (pos != production.end()) {
                size_t index = distance(production.begin(), pos);

                // Case: A -> αBβ, where B is non-terminal
                if (index + 1 < production.size()) {
                    const string& nextSymbol = production[index + 1];

                    // Compute First set of the symbols following B in the production
                    set<char> firstSet;

                    // Note: First of a terminal is the terminal itself
                    if (islower(nextSymbol[0])) {
                        firstSet.insert(nextSymbol[0]);
                    } else {
                        // If it's a non-terminal, use the precomputed First set
                        const set<char>& nextFirstSet = computedFirstSets[nextSymbol];
                        firstSet.insert(nextFirstSet.begin(), nextFirstSet.end());
                    }

                    // Add First set excluding epsilon to Follow set
                    followSet.insert(firstSet.begin(), firstSet.end());
                    followSet.erase(EPSILON[0]);

                    // If B can derive epsilon, add Follow(A) to Follow(B)
                    if (nonTerminalHasEpsilon(nextSymbol)) {
                        //TODO : not always it could have another Non terminal after this symbol
                        const set<char>& followASet = computeFollow(rule.nonTerminal);
                        followSet.insert(followASet.begin(), followASet.end());
                    }
                } else {
                    // Case: A -> αB, where B is the last symbol
                    // Add Follow(A) to Follow(B)
                    const set<char>& followASet = computeFollow(rule.nonTerminal);
                    followSet.insert(followASet.begin(), followASet.end());
                }
            }
        }
    }

    // Save the computed Follow set
    computedFollowSets[nonTerminal] = followSet;

    return followSet;
}
// Function to compute First sets for each non-terminal
void  GrammarChecker::computeFirstSets(unordered_map<string, set<char>>& firstSets) {
    for (const string& nonTerminal : nonTerminals ) {
        firstSets[nonTerminal] = computeFirst(nonTerminal);
    }
}
// Function to compute Follow sets for each non-terminal
void  GrammarChecker::computeFollowSets(unordered_map<string, set<char>>& followSets) {
    for (const string& nonTerminal : nonTerminals ) {
        followSets[nonTerminal] = computeFollow(nonTerminal);
    }
}
bool GrammarChecker::isLL1Grammar() {

    computeFirstSets(computedFirstSets);

    // Compute Follow sets for each non-terminal
    computeFollowSets(computedFollowSets);

    return true;
}
