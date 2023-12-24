#include "Parser/CFGReader.h"
#include "Parser/FirstAndFollowGenerator.h"
#include "Parser/FirstAndFollowGeneratorUtility.h"
#include "Parser/Production.h"
#include "constants.h"

FirstAndFollowGenerator::FirstAndFollowGenerator(
        const std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar) {
    for (const auto &entry: grammar) {
        const std::string &nonTerminal = entry.first;
        const std::vector<std::vector<std::string>> &productions = entry.second;
        productionVector.push_back({nonTerminal, productions});
    }
    nonTerminals = collectNonTerminals(productionVector);
}

std::set<std::pair<std::string, Production>, CompareFirst>
FirstAndFollowGenerator::computeFirst(const std::string &nonTerminal) {
    std::set<std::pair<std::string, Production>, CompareFirst> firstSet;

    for (const Production &rule: productionVector) {
        if (rule.nonTerminal == nonTerminal) {
            for (const auto &production: rule.productions) {
                for (const std::string &symbol: production) {
                    if (nonTerminals.find(symbol) != nonTerminals.end()) {
                        // Handle non-terminal symbols
                        const std::string &symbolStr = symbol;

                        // Compute First set for the non-terminal
                        const std::set<std::pair<std::string, Production>, CompareFirst> &nonTerminalFirstSet = computeFirst(
                                symbolStr);
                        // insert all the first from nonTerminalFirstSet but with the production in the for loop not the one coming with the variable
                        for (const auto &pair: nonTerminalFirstSet) {
                            firstSet.insert({pair.first, {nonTerminal, {production}}});
                        }

                        // Check if the non-terminal has an epsilon production
                        if (!nonTerminalHasEpsilon(symbolStr)) {
                            // If it doesn't have epsilon, break the loop
                            break;
                        }
                    } else {
                        // Handle terminal symbols
                        firstSet.insert({symbol, {nonTerminal, {production}}});
                        //firstSet.insert(symbol);
                        // Break the loop for terminal symbols
                        break;
                    }
                }
            }
        }
    }

    return firstSet;
}

bool FirstAndFollowGenerator::nonTerminalHasEpsilon(const std::string &nonTerminal) {
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

std::set<std::string> FirstAndFollowGenerator::computeFollow(const std::string &nonTerminal) {
    // If Follow set is already computed, return it
    if (computedFollowSets.find(nonTerminal) != computedFollowSets.end()) {
        return computedFollowSets[nonTerminal];
    }

    std::set<std::string> followSet;


    // The start symbol has $ (end of input) in its Follow set
    //1) FOLLOW(S) = { $ }   // where S is the starting Non-Terminal
    if (nonTerminal == CFGReader::start_symbol) {
        followSet.insert("$");
    }
    // Mark the Follow set as computed to handle recursive calls
    computedFollowSets[nonTerminal] = followSet;

    for (const Production &rule: productionVector) {
        for (const auto &production: rule.productions) {
            auto pos = find(production.begin(), production.end(), nonTerminal);
            if (pos != production.end()) {
                size_t index = distance(production.begin(), pos);

                // Case: A -> αBβ, where B is non-terminal
                if (index + 1 < production.size()) {
                    const std::string &nextSymbol = production[index + 1];

                    // Compute First set of the symbols following B in the production
                    std::set<std::string> firstSet;

                    //2) If A -> pBq is a production, where p, B and q are any grammar symbols,
                    //   then everything in FIRST(q)  except Є is in FOLLOW(B).
                    // Note: First of a terminal is the terminal itself
                    if (nonTerminals.find(nextSymbol) == nonTerminals.end()) {
                        firstSet.insert(nextSymbol);
                    } else {
                        // If it's a non-terminal, use the precomputed First set
                        const std::set<std::string> &nextFirstSet = computedFirstSetsWithoutProductions[nextSymbol];
                        firstSet.insert(nextFirstSet.begin(), nextFirstSet.end());
                    }
                    // Add First set excluding epsilon to Follow set
                    followSet.insert(firstSet.begin(), firstSet.end());
                    followSet.erase(EPSILON);


                    // If B can derive epsilon, add Follow(A) to Follow(B)
                    if (firstSet.find(EPSILON) != firstSet.end() || nonTerminalHasEpsilon(nextSymbol)) {
                        //5) If A->pBqd is a production and FIRST(q) contains Є,
                        //   then FOLLOW(B) contains { FIRST(q) – Є } U First(d)
                        size_t qIndex = index + 2;

                        std::set<std::string> firstQSet;
                        while (qIndex < production.size() && nonTerminals.find(production[qIndex]) != nonTerminals.end()
                               && (computedFirstSetsWithoutProductions[production[qIndex]].find(EPSILON) !=
                                   computedFirstSetsWithoutProductions[production[qIndex]].end() ||
                                   nonTerminalHasEpsilon(production[qIndex]))) {
                            const std::string &qSymbol = production[qIndex];
                            if (nonTerminals.find(qSymbol) == nonTerminals.end()) {
                                followSet.insert(production[qIndex]);
                                qIndex++;
                                computedFollowSets[nonTerminal] = followSet;

                                break;
                            }
                            const std::set<std::string> &qFirstSet = computedFirstSetsWithoutProductions[qSymbol];
                            for (std::string symbol: qFirstSet) {
                                if (symbol != EPSILON) {
                                    firstQSet.insert(symbol);
                                }
                            }
                            qIndex++;
                        }

                        followSet.insert(firstQSet.begin(), firstQSet.end());
                        if (qIndex < production.size() && nonTerminals.find(production[qIndex]) == nonTerminals.end())
                            followSet.insert(production[qIndex]);
                        else {
                            //4) If A->pBq is a production and FIRST(q) contains Є,
                            //   then FOLLOW(B) contains { FIRST(q) – Є } U FOLLOW(A)
                            qIndex--;
                            if (qIndex < production.size() &&
                                (computedFirstSetsWithoutProductions[production[qIndex]].find(EPSILON) !=
                                 computedFirstSetsWithoutProductions[production[qIndex]].end()
                                 || nonTerminalHasEpsilon(production[qIndex]))) {
                                const std::set<std::string> &followASet = computeFollow(rule.nonTerminal);
                                followSet.insert(followASet.begin(), followASet.end());
                            }
                        }
                    }
                    computedFollowSets[nonTerminal] = followSet;

                } else {
                    // Case: A -> αB, where B is the last symbol
                    // Add Follow(A) to Follow(B)
                    // If A->pB is a production, then everything in FOLLOW(A) is in FOLLOW(B).
                    const std::set<std::string> &followASet = computeFollow(rule.nonTerminal);

                    if (!followASet.empty()) {
                        // If followASet is not empty, add it to followSet and computedFollowSets
                        followSet.insert(followASet.begin(), followASet.end());
                        computedFollowSets[nonTerminal] = followSet;
                    } else {
                        // If followASet is empty, remove rule.nonTerminal from computedFollowSets
                        //don't remember you calculated before
                        computedFollowSets.erase(rule.nonTerminal);
                    }
                }
            }
        }
    }

    // Save the computed Follow set
    computedFollowSets[nonTerminal] = followSet;

    return followSet;
}

void FirstAndFollowGenerator::computeFirstSets(
        std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> &firstSets) {
    for (const std::string &nonTerminal: nonTerminals) {
        firstSets[nonTerminal] = computeFirst(nonTerminal);
    }
}

void FirstAndFollowGenerator::computeFollowSets(std::unordered_map<std::string, std::set<std::string>> &followSets) {
    for (const std::string &nonTerminal: nonTerminals) {
        followSets[nonTerminal] = computeFollow(nonTerminal);
    }
    iterateAndMergeFollowSets();
}

void FirstAndFollowGenerator::iterateAndMergeFollowSets() {
    bool followSetsChanged = true;

    while (followSetsChanged) {
        followSetsChanged = false;
        for (const std::string &nonTerminal: nonTerminals) {
            for (const Production &rule: productionVector) {
                for (const auto &production: rule.productions) {
                    auto pos =
                            find(production.begin(), production.end(), nonTerminal);
                    if (pos != production.end()) {
                        size_t index = distance(production.begin(), pos);

                        // Case: A -> αBβ, where B is non-terminal
                        if (index + 1 == production.size()) {
                            const std::set<std::string> &followASet =
                                    computedFollowSets[rule.nonTerminal];
                            auto initialSize =
                                    computedFollowSets[nonTerminal].size();
                            computedFollowSets[nonTerminal].insert(
                                    followASet.begin(), followASet.end());
                            auto newSize =
                                    computedFollowSets[nonTerminal].size();
                            followSetsChanged |= initialSize != newSize;
                        }
                    }
                }
            }
        }
    }
}

void FirstAndFollowGenerator::compute() {

    computeFirstSets(computedFirstSets);

    for (auto &entry: computedFirstSets) {
        std::string nonTerminal = entry.first;
        std::set<std::string> firstSet;
        for (auto &pair: entry.second) {
            firstSet.insert(pair.first);
        }
        computedFirstSetsWithoutProductions[nonTerminal] = firstSet;
    }

    // Compute Follow sets for each non-terminal
    computeFollowSets(computedFollowSets);
}
