#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "RuleType.h"

class Rules {
public:
    Rules();

    ~Rules();

    void add_rule(RuleType type, std::string *name, std::string *expression);

    void print_rules();

private:
    std::unordered_map<std::string, std::string> *regular_expressions;
    std::unordered_map<std::string, std::string> *regular_definitions;
    std::vector<std::string> *keywords;
    std::vector<std::string> *punctuations;

    void add_regular_expression(std::string *name, std::string *expression);

    void add_regular_definition(std::string *name, std::string *definition);

    void add_keyword(std::string *keyword);

    void add_punctuation(std::string *punctuation);
};