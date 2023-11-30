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
#include "Production.h"
using namespace std;
class GrammarChecker {
public:
    GrammarChecker(const unordered_map<string, vector<vector<string>>>& grammar);
    const unordered_map<string, set<char>>& getFirstSets() const {
        return computedFirstSets;
    }

    const unordered_map<string, set<char>>& getFollowSets() const {
        return computedFollowSets;
    }
    bool isLL1Grammar();

private:
    vector<Production> productionVector;
    unordered_map<string, set<char>> computedFirstSets;
    unordered_map<string, set<char>> computedFollowSets;

    set<char> computeFirst(char nonTerminal);
    set<char> computeFollow(char nonTerminal);

    bool hasCommonElements();
    bool hasCommonIntersection();
};

#endif // GRAMMAR_CHECKER_H
