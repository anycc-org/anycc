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

    void parseFile(std::ifstream *file);

    static RuleType checkType(std::string *basicString);

    void buildRule(std::string &pString, RuleType type);

    void addKeywords(std::string &string, std::string &delimiter);

    void addPunctuations(std::string pString);

    void addRegularDefinitionOrExpression(std::string &pString, RuleType type);

    void addKeyword(std::string &pString);

    void addPunctuation(std::string &pString);

    void buildRules(std::ifstream *file);
};