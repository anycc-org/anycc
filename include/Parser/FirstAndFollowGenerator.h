
#pragma once
#ifndef FIRST_AND_FOLLOW_GENERATOR_H
#define FIRST_AND_FOLLOW_GENERATOR_H

#include "Parser/Production.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

struct CompareFirst {
    bool
    operator()(const std::pair<std::string, Production> &lhs, const std::pair<std::string, Production> &rhs) const {
        return lhs.first < rhs.first;
    }
};

class FirstAndFollowGenerator {
public:
    FirstAndFollowGenerator(const std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar);

    const std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> &
    getFirstSets() const {
        return computedFirstSets;
    }

    const std::unordered_map<std::string, std::set<std::string>> &getFollowSets() const {
        return computedFollowSets;
    }

    const std::set<std::string> &getNonTerminals() const {
        return nonTerminals;
    }

    void compute();

    void printFirstSets();

    void printFollowSets();

private:
    std::vector<Production> productionVector;
    std::set<std::string> nonTerminals;
    std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> computedFirstSets;
    std::unordered_map<std::string, std::set<std::string>> computedFirstSetsWithoutProductions;
    std::unordered_map<std::string, std::set<std::string>> computedFollowSets;

    std::set<std::pair<std::string, Production>, CompareFirst> computeFirst(const std::string &nonTerminal);

    std::set<std::string> computeFollow(const std::string &nonTerminal);

    bool nonTerminalHasEpsilon(const std::string &nonTerminal);

    void computeFirstSets(
            std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> &firstSets);

    void computeFollowSets(std::unordered_map<std::string, std::set<std::string>> &followSets);

    void iterateAndMergeFollowSets();

};

#endif // FIRST_AND_FOLLOW_GENERATOR_H
