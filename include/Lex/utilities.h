#pragma once

#include <vector>
#include "RuleType.h"
#include "InputReader.h"


class utilities {
public:
    static std::vector<SubstringInfo>
    findAllLongestSubstringIndices(std::string *input, std::set<std::string> *substrings);

    static void fix_spaces_given_type(const std::unordered_map<std::string, std::pair<std::string, int>> *regular_rules, Rules *rules,
                               std::set<std::string> *non_terminal_symbols, RuleType type);

    static int
    detect_concat_then_add_spaces(std::string *expression, const std::vector<SubstringInfo> &substringInfoVec,
                                  int offset, int i);

    static void add_space_after_and_before_braces(std::string *expression);

    static void fix_spaces(Rules *rules, std::set<std::string> *non_terminal_symbols);

    static std::vector<Token*> *convert_map_to_vector(std::unordered_map<std::string, std::pair<std::string, int>> *map);

    static void delete_vector_of_tokens(std::vector<Token*> *vector);
};
