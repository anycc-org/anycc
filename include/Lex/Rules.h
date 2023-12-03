#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "RuleType.h"
#include "Token.h"

class Rules {
public:
    Rules();

    ~Rules();

    void add_rule(RuleType type, std::string *name, std::string *expression);

    void print_rules();

    std::unordered_map<std::string, std::pair<std::string, int>> *getRegularExpressionsMap() const;

    std::unordered_map<std::string, std::pair<std::string, int>> *getRegularDefinitionsMap() const;

    std::vector<Token*> *getRegularExpressions() const;

    std::vector<Token*> *getRegularDefinitions() const;

    std::vector<std::string> *getKeywords() const;

    std::vector<std::string> *getPunctuations() const;

    void setRegularExpressionsTokensVector(std::vector<Token*> *regularExpressionsTokensVector);

    void setRegularDefinitionsTokensVector(std::vector<Token*> *regularDefinitionsTokensVector);

private:
    std::unordered_map<std::string, std::pair<std::string, int>> *regular_expressions_map;
    std::vector<Token*> *regular_expressions_tokens_vector;
    std::unordered_map<std::string, std::pair<std::string, int>> *regular_definitions_map;
    std::vector<Token*> *regular_definitions_tokens_vector;
    std::vector<std::string> *keywords;
    std::vector<std::string> *punctuations;
    int expression_id, definition_id;

    void add_regular_expression(std::string *name, std::string *expression);

    void add_regular_definition(std::string *name, std::string *definition);

    void add_keyword(std::string *keyword);

    void add_punctuation(std::string *punctuation);
};