#pragma once

#include <vector>
#include "RuleType.h"
#include "InputReader.h"


class Utilities {
public:
    static std::vector<SubstringInfo>
    findAllLongestSubstringIndices(std::string *input, std::set<std::string> *substrings);

    static void fixSpacesGivenType(std::unordered_map<std::string, std::pair<std::string, int>> regular_rules, Rules *rules,
                                   std::set<std::string> *non_terminal_symbols, RuleType type);

    static int
    detectConcatThenAddSpaces(std::string *expression, const std::vector<SubstringInfo> &substringInfoVec,
                              int offset, int i);

    static void addSpaceAfterAndBeforeBraces(std::string *expression);

    static void fixSpaces(Rules *rules, std::set<std::string> *non_terminal_symbols);

    static std::vector<Token*> convertMapToVector(const std::unordered_map<std::string, std::pair<std::string, int>>& map);

    static void deleteVectorOfTokens(std::vector<Token*> *vector);
};
