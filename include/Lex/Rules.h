#ifndef ANYCC_RULES_H
#define ANYCC_RULES_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Lex/RuleType.h"
#include "Lex/Token.h"

class Rules {
public:
    Rules();

    ~Rules();

    void addRule(RuleType type, std::string expression, std::string name = "");

    void printRules();

    std::unordered_map<std::string, std::pair<std::string, int>> &getRegularExpressionsMap();

    std::unordered_map<std::string, std::pair<std::string, int>> &getRegularDefinitionsMap();

    std::vector<Token *> &getRegularExpressions();

    std::vector<Token *> &getRegularDefinitions();

    std::vector<std::string> &getKeywords();

    std::vector<std::string> &getPunctuations();

    std::unordered_map<std::string, int> &getTokensPriority();

    std::vector<std::string> getTokens();

    void setRegularExpressionsTokensVector(std::vector<Token *> regular_expressions_vector);

    void setRegularDefinitionsTokensVector(std::vector<Token *> regular_definitions_vector);

private:
    std::unordered_map<std::string, int> tokens_priority;
    std::unordered_map<std::string, std::pair<std::string, int>> regular_expressions_map;
    std::vector<Token *> regular_expressions_tokens_vector;
    std::unordered_map<std::string, std::pair<std::string, int>> regular_definitions_map;
    std::vector<Token *> regular_definitions_tokens_vector;
    std::vector<std::string> keywords;
    std::vector<std::string> punctuations;
    int expression_id;
    int definition_id;
    int priority;

    void addRegularExpression(std::string &name, std::string &expression);

    void addRegularDefinition(std::string &name, std::string &definition);

    void addKeyword(std::string &keyword);

    void addPunctuation(std::string &punctuation);
};

#endif //ANYCC_RULES_H