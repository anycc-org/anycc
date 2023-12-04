#include <iostream>
#include <utility>
#include "Rules.h"
#include "Utilities.h"

Rules::Rules() {
    regular_expressions_map = std::unordered_map<std::string, std::pair<std::string, int>>();
    regular_definitions_map = std::unordered_map<std::string, std::pair<std::string, int>>();
    regular_expressions_tokens_vector = std::vector<Token *>();
    regular_definitions_tokens_vector = std::vector<Token *>();
    keywords = std::vector<std::string>();
    punctuations = std::vector<std::string>();
    expression_id = 0;
    definition_id = 0;
}

Rules::~Rules() {
    Utilities::deleteVectorOfTokens(&regular_definitions_tokens_vector);
    Utilities::deleteVectorOfTokens(&regular_expressions_tokens_vector);
}

void Rules::addRule(RuleType type, std::string expression, std::string name) {
    switch (type) {
        case RuleType::REGULAR_EXPRESSION:
            addRegularExpression(name, expression);
            break;
        case RuleType::REGULAR_DEFINITION:
            addRegularDefinition(name, expression);
            break;
        case RuleType::KEYWORDS:
            addKeyword(expression);
            break;
        case RuleType::PUNCTUATION:
            addPunctuation(expression);
            break;
        default:
            break;
    }

}

void Rules::printRules() {
    std::cout << "\n\n" << "Regular Expressions:" << "\n";
    for (const auto &re: regular_expressions_tokens_vector) {
        std::cout << *re->getKey() << " : " << *re->getValue() << std::endl;
    }
    std::cout << "\n\n";

    std::cout << "Regular Definitions:" << "\n";
    for (const auto &rd: regular_definitions_tokens_vector) {
        std::cout << *rd->getKey() << " = " << *rd->getValue() << std::endl;
    }
    std::cout << "\n\n";

    std::cout << "Keywords:" << "\n";
    for (auto &keyword: keywords) {
        std::cout << keyword << std::endl;
    }
    std::cout << "\n\n";

    std::cout << "Punctuations:" << "\n";
    for (auto &punc: punctuations) {
        std::cout << punc << std::endl;
    }
}

std::unordered_map<std::string, std::pair<std::string, int>> &Rules::getRegularExpressionsMap() {
    return regular_expressions_map;
}

std::unordered_map<std::string, std::pair<std::string, int>> &Rules::getRegularDefinitionsMap() {
    return regular_definitions_map;
}

std::vector<Token *> &Rules::getRegularExpressions() {
    return regular_expressions_tokens_vector;
}

std::vector<Token *> &Rules::getRegularDefinitions() {
    return regular_definitions_tokens_vector;
}

std::vector<std::string> &Rules::getKeywords() {
    return keywords;
}

std::vector<std::string> &Rules::getPunctuations() {
    return punctuations;
}

void Rules::setRegularExpressionsTokensVector(std::vector<Token *> regular_expressions_vector) {
    regular_expressions_tokens_vector = std::move(regular_expressions_vector);
}

void Rules::setRegularDefinitionsTokensVector(std::vector<Token *> regular_definitions_vector) {
    regular_definitions_tokens_vector = std::move(regular_definitions_vector);
}

void Rules::addPunctuation(std::string &punctuation) {
    this->punctuations.push_back(punctuation);
}

void Rules::addRegularDefinition(std::string &name, std::string &definition) {
    if (regular_definitions_map.find(name) != regular_definitions_map.end()) {
        regular_definitions_map[name] = {definition, regular_definitions_map[name].second};
        return;
    }
    regular_definitions_map[name] = {definition, definition_id++};
}

void Rules::addRegularExpression(std::string &name, std::string &expression) {
    if (regular_expressions_map.find(name) != regular_expressions_map.end()) {
        regular_expressions_map[name] = {expression, regular_expressions_map[name].second};
        return;
    }

    regular_expressions_map[name] = {expression, expression_id++};
}

void Rules::addKeyword(std::string &keyword) {
    this->keywords.push_back(keyword);
}
