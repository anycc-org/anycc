#pragma once

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <set>
#include "Rules.h"
#include "RuleType.h"

struct SubstringInfo {
    int start;
    int end;
};

inline bool compareSubstringInfo(const SubstringInfo &a, const SubstringInfo &b) {
    return a.start < b.start;
}

class InputReader {
public:
    explicit InputReader(std::string *rules_file_name, Rules *rules);

    ~InputReader();

private:
    std::set<std::string> *non_terminal_symbols;
    Rules *rules;

    void parse_file(std::ifstream *file);

    static RuleType check_type(std::string *basicString);

    void build_rule(RuleType type, std::string *pString);

    void add_keywords(std::string *string, std::string *delimiter);

    void add_punctuations(std::string *pString);

    void add_regular_definition_or_expression(const std::string *pString, RuleType type);

    void add_keyword(const std::string *pString);

    void add_punctuation(const std::string *pString);

    void build_rules(std::ifstream *file);
};