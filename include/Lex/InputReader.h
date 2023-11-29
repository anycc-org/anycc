#pragma once

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include "Rules.h"
#include "RuleType.h"

class InputReader {
public:
    explicit InputReader(std::string *rules_file_name, Rules *rules);

    ~InputReader();

private:
    Rules *rules;

    void parse_file(std::ifstream *file);

    static RuleType check_type(std::string *basicString);

    void build_rule(RuleType type, std::string *pString);

    void split_string(std::string *string, std::string *delimiter);

    void split_punctuation(std::string *pString);
};