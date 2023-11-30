//
// Created by abdel on 30/11/2023.
//
#pragma once
#ifndef ANYCC_GRAMMARCHECKER_H
#define ANYCC_GRAMMARCHECKER_H



#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>

// Forward declaration
struct Production;

class GrammarChecker {
public:
    GrammarChecker(const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar);
    bool isLL1Grammar();

private:
    std::unordered_map<std::string, std::set<char>> computeFirst(char nonTerminal);
    std::set<char> computeFollow(char nonTerminal);

    bool hasCommonElements(const std::unordered_map<std::string, std::set<char>>& sets);
    bool hasCommonIntersection();

    std::vector<Production> productionVector;
    std::unordered_map<std::string, std::set<char>> firstSets;
    std::unordered_map<std::string, std::set<char>> followSets;
};

