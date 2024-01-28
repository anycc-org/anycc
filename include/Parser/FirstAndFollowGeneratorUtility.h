#ifndef FIRSTANDFOLLOWGENERATORUTILITY_H
#define FIRSTANDFOLLOWGENERATORUTILITY_H

#include "Parser/Production.h"
#include "SubstringInfo.h"
#include <set>
#include <string>
#include <vector>

std::vector<SubstringInfo>
findAllLongestSubstringIndices(const std::string &input, const std::set<std::string> &substrings);

bool isNT(const std::string &s, std::set<std::string> &nonTerminals);

std::set<std::string> collectNonTerminals(const std::vector<Production> &grammar);

std::string getLongestUpperCaseSequence(const std::string &str);

#endif // FIRSTANDFOLLOWGENERATORUTILITY_H
