//
// Created by abdel on 30/11/2023.
//
#pragma once
#ifndef GRAMMAR_CHECKER_H
#define GRAMMAR_CHECKER_H

#include "Parser/Production.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

class FirstAndFollowGenerator {
public:
  FirstAndFollowGenerator(const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar);

  const std::unordered_map<std::string, std::set<std::string>>& getFirstSets() const {
    return computedFirstSets;
  }

  const std::unordered_map<std::string, std::set<std::string>>& getFollowSets() const {
    return computedFollowSets;
  }

  bool isLL1Grammar();

private:
  std::vector<Production> productionVector;
  std::set<std::string> nonTerminals;
  std::unordered_map<std::string, std::set<std::string>> computedFirstSets;
  std::unordered_map<std::string, std::set<std::string>> computedFollowSets;

  std::set<std::string> computeFirst(const std::string& nonTerminal);
  std::set<std::string> computeFollow(const std::string& nonTerminal);
  bool nonTerminalHasEpsilon(const std::string& nonTerminal);
  void computeFirstSets(std::unordered_map<std::string, std::set<std::string>>& firstSets);
  void computeFollowSets(std::unordered_map<std::string, std::set<std::string>>& followSets);
};

#endif // GRAMMAR_CHECKER_H
