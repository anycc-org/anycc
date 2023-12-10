#include "Production.h"
#include "GrammarChecker.h"
#include "constants.h"
#include "GrammarCheckerUtility.h"

GrammarChecker::GrammarChecker(const unordered_map<string, vector<vector<string>>> &grammar) {
    for (const auto &entry: grammar) {
        const string &nonTerminal = entry.first;
        const vector<vector<string>> &productions = entry.second;

        productionVector.push_back({nonTerminal, productions});
    }

    nonTerminals = collectNonTerminals(productionVector);
}

set<string> GrammarChecker::computeFirst(const string &nonTerminal) {
    set<string> firstSet;

    for (const Production &rule: productionVector) {
        if (rule.nonTerminal == nonTerminal) {
            for (const auto &production: rule.productions) {
                for (const string &symbol: production) {
                    if (nonTerminals.count(symbol)) {
                        // Handle non-terminal symbols
                        const string &symbolStr = symbol;

                        // Compute First set for the non-terminal
                        const set<string> &nonTerminalFirstSet = computeFirst(symbolStr);
                        firstSet.insert(nonTerminalFirstSet.begin(), nonTerminalFirstSet.end());

                        // Check if the non-terminal has an epsilon production
                        if (!nonTerminalHasEpsilon(symbolStr)) {
                            // If it doesn't have epsilon, break the loop
                            break;
                        }
                    } else {
                        // Handle terminal symbols
                        firstSet.insert(symbol);
                        // Break the loop for terminal symbols
                        break;
                    }
                }
            }
        }
    }

    return firstSet;
}

bool GrammarChecker::nonTerminalHasEpsilon(const string &nonTerminal) {
    // Check if the non-terminal has an epsilon production
    for (const Production &rule: productionVector) {
        if (rule.nonTerminal == nonTerminal) {
            for (const auto &production: rule.productions) {
                if (production.size() == 1 && production[0] == EPSILON) {
                    return true;
                }
            }
        }
    }
    return false;
}

set<string> GrammarChecker::computeFollow(const string &nonTerminal) {
    // If Follow set is already computed, return it
    if (computedFollowSets.find(nonTerminal) != computedFollowSets.end()) {
        return computedFollowSets[nonTerminal];
    }

    set<string> followSet;

    // Mark the Follow set as computed to handle recursive calls
    computedFollowSets[nonTerminal] = followSet;

    // The start symbol has $ (end of input) in its Follow set
    //1) FOLLOW(S) = { $ }   // where S is the starting Non-Terminal
    if (nonTerminal == START_SYMBOL) {
        followSet.insert("$");
    }

    for (const Production &rule: productionVector) {
        for (const auto &production: rule.productions) {
            auto pos = find(production.begin(), production.end(), nonTerminal);
            if (pos != production.end()) {
                size_t index = distance(production.begin(), pos);

                // Case: A -> αBβ, where B is non-terminal
                if (index + 1 < production.size()) {
                    const string &nextSymbol = production[index + 1];

                    // Compute First set of the symbols following B in the production
                    set<string> firstSet;

                    //2) If A -> pBq is a production, where p, B and q are any grammar symbols,
                    //   then everything in FIRST(q)  except Є is in FOLLOW(B).
                    // Note: First of a terminal is the terminal itself
                    if (!nonTerminals.count(nextSymbol)) {
                        firstSet.insert(nextSymbol);
                    } else {
                        // If it's a non-terminal, use the precomputed First set
                        const set<string> &nextFirstSet = computedFirstSets[nextSymbol];
                        firstSet.insert(nextFirstSet.begin(), nextFirstSet.end());
                    }
                    // Add First set excluding epsilon to Follow set
                    followSet.insert(firstSet.begin(), firstSet.end());
                    followSet.erase(EPSILON);


                    // If B can derive epsilon, add Follow(A) to Follow(B)
                    if (firstSet.count(EPSILON) || nonTerminalHasEpsilon(nextSymbol)) {
                        //5) If A->pBqd is a production and FIRST(q) contains Є,
                        //   then FOLLOW(B) contains { FIRST(q) – Є } U First(d)
                        size_t qIndex = index + 2;

                        set<string> firstQSet;
                        while (qIndex < production.size() && nonTerminals.count(production[qIndex])
                               && (computedFirstSets[production[qIndex]].count(EPSILON) ||
                                   nonTerminalHasEpsilon(production[qIndex]))) {
                            const string &qSymbol = production[qIndex];
                            if (!nonTerminals.count(qSymbol)) {
                                followSet.insert(production[qIndex]);
                                qIndex++;
                                break;
                            }
                            const set<string> &qFirstSet = computedFirstSets[qSymbol];
                            for (string symbol: qFirstSet) {
                                if (symbol != EPSILON) {
                                    firstQSet.insert(symbol);
                                }
                            }
                            qIndex++;
                        }

                        followSet.insert(firstQSet.begin(), firstQSet.end());
                        if (qIndex < production.size() && !nonTerminals.count(production[qIndex]))
                            followSet.insert(production[qIndex]);
                        else {
                            //4) If A->pBq is a production and FIRST(q) contains Є,
                            //   then FOLLOW(B) contains { FIRST(q) – Є } U FOLLOW(A)
                            qIndex--;
                            if (qIndex < production.size() &&
                                (computedFirstSets[production[qIndex]].count(EPSILON)
                                 || nonTerminalHasEpsilon(production[qIndex]))) {
                                const set<string> &followASet = computeFollow(rule.nonTerminal);
                                followSet.insert(followASet.begin(), followASet.end());
                            }
                        }
                    }
                } else {
                    // Case: A -> αB, where B is the last symbol
                    // Add Follow(A) to Follow(B)
                    // If A->pB is a production, then everything in FOLLOW(A) is in FOLLOW(B).
                    const set<string> &followASet = computeFollow(rule.nonTerminal);
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
void GrammarChecker::computeFirstSets(unordered_map<string, set<string>> &firstSets) {
    for (const string &nonTerminal: nonTerminals) {
        firstSets[nonTerminal] = computeFirst(nonTerminal);
    }
}

// Function to compute Follow sets for each non-terminal
void GrammarChecker::computeFollowSets(unordered_map<string, set<string>> &followSets) {
    for (const string &nonTerminal: nonTerminals) {
        followSets[nonTerminal] = computeFollow(nonTerminal);
    }
}

bool GrammarChecker::isLL1Grammar() {

    computeFirstSets(computedFirstSets);

    // Compute Follow sets for each non-terminal
    computeFollowSets(computedFollowSets);

    return true;
}
