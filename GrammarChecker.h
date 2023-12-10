//
// Created by abdel on 30/11/2023.
//
#pragma once
#ifndef GRAMMAR_CHECKER_H
#define GRAMMAR_CHECKER_H

#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "Production.h"
using namespace std;
class GrammarChecker {
public:
    GrammarChecker(const unordered_map<string, vector<vector<string>>>& grammar);
    const unordered_map<string, set<string>>& getFirstSets() const {
        return computedFirstSets;
    }

    const unordered_map<string, set<string>>& getFollowSets() const {
        return computedFollowSets;
    }
    bool isLL1Grammar();

private:
    vector<Production> productionVector;
    set<string> nonTerminals;
    unordered_map<string, set<string>> computedFirstSets;
    unordered_map<string, set<string>> computedFollowSets;
    set<string> computeFirst(const string& nonTerminal);
    set<string> computeFollow(const string& nonTerminal);
    bool nonTerminalHasEpsilon(const string& nonTerminal);
    void  computeFirstSets(unordered_map<string, set<string>>& firstSets) ;
    void computeFollowSets(unordered_map<string, set<string>>& followSets);
};

#endif // GRAMMAR_CHECKER_H
