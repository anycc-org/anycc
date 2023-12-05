#pragma once

#include <vector>
#include "RuleType.h"
#include "InputReader.h"
#include "Operator.h"


class Utilities {
public:
    static std::vector<SubstringInfo>
    findAllLongestSubstringIndices(std::string *input, std::set<std::string> *substrings);

    static void fixConcatGivenType(const std::unordered_map<std::string, std::pair<std::string, int>>& regular_rules, Rules *rules,
                                   std::set<std::string> *non_terminal_symbols, RuleType type);

    static int
    detectConcatThenAddSpaces(std::string *expression, const std::vector<SubstringInfo> &substringInfoVec,
                              int offset, int i);

    static void addSpaceAfterAndBeforeBraces(std::string *expression);

    static void fixConcat(Rules *rules, std::set<std::string> *non_terminal_symbols);

    static std::vector<Token*> convertMapToVector(const std::unordered_map<std::string, std::pair<std::string, int>>& map);

    static void deleteVectorOfTokens(std::vector<Token*> *vector);

    static bool isCloseBrace(const std::string *expression, int i);

    static bool isConcat(const std::string *expression, int i);

    static bool isOr(const std::string *expression, int i);

    static bool isKleeneClosure(const std::string *expression, int i);

    static bool isPositiveClosure(const std::string *expression, int i);

    static bool isOpenBrace(const std::string *expression, int i);
};