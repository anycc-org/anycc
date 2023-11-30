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

class GrammarChecker {
public:
    GrammarChecker(const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar);

    bool isLL1Grammar();

private:
    std::vector<Production> productionVector;
    std::unordered_map<std::string, set<char>> computedFirstSets;
    std::unordered_map<std::string, set<char>> computedFollowSets;

    std::set<char> computeFirst(char nonTerminal);
    std::set<char> computeFollow(char nonTerminal);

    bool hasCommonElements(const std::unordered_map<std::string, std::set<char>>& sets);
    bool hasCommonIntersection(const std::unordered_map<std::string, std::set<char>>& sets);

};

#endif // GRAMMAR_CHECKER_H

