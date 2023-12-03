#include <iostream>
#include "Rules.h"

Rules::Rules() {
    regular_expressions_map = new std::unordered_map<std::string, std::pair<std::string, int>>();
    regular_definitions_map = new std::unordered_map<std::string, std::pair<std::string, int>>();
    keywords = new std::vector<std::string>();
    punctuations = new std::vector<std::string>();
    expression_id = 0;
    definition_id = 0;
}

Rules::~Rules() {
    delete regular_expressions_map;
    delete regular_definitions_map;
    delete regular_expressions_tokens_vector;
    delete regular_definitions_tokens_vector;
    delete keywords;
    delete punctuations;
}

void Rules::add_punctuation(std::string *punctuation) {
    this->punctuations->push_back(*punctuation);
}

void Rules::add_regular_definition(std::string *name, std::string *definition) {
    if (regular_definitions_map->find(*name) != regular_definitions_map->end()) {
        (*regular_definitions_map)[*name] = {*definition, (*regular_definitions_map)[*name].second};
        return;
    }
    (*regular_definitions_map)[*name] = {*definition, definition_id++};
}

void Rules::add_regular_expression(std::string *name, std::string *expression) {
    if (regular_expressions_map->find(*name) != regular_expressions_map->end()) {
        (*regular_expressions_map)[*name] = {*expression, (*regular_expressions_map)[*name].second};
        return;
    }

    (*regular_expressions_map)[*name] = {*expression, expression_id++};
}

void Rules::add_keyword(std::string *keyword) {
    this->keywords->push_back(*keyword);
}

void Rules::add_rule(RuleType type, std::string *name, std::string *expression) {
    switch (type) {
        case RuleType::REGULAR_EXPRESSION:
            add_regular_expression(name, expression);
            break;
        case RuleType::REGULAR_DEFINITION:
            add_regular_definition(name, expression);
            break;
        case RuleType::KEYWORDS:
            add_keyword(expression);
            break;
        case RuleType::PUNCTUATION:
            add_punctuation(expression);
            break;
        default:
            break;
    }

}

void Rules::print_rules() {
    std::cout << "\n\n" << "Regular Expressions:" << "\n";
    for (auto re : *regular_expressions_tokens_vector) {
        std::cout << *re->getKey() << " : " << *re->getValue() << std::endl;
    }
    std::cout << "\n\n";

    std::cout << "Regular Definitions:" << "\n";
    for (auto rd : *regular_definitions_tokens_vector) {
        std::cout << *rd->getKey() << " = " << *rd->getValue() << std::endl;
    }
    std::cout << "\n\n";

    std::cout << "Keywords:" << "\n";
    for (auto &keyword : *keywords) {
        std::cout << keyword << std::endl;
    }
    std::cout << "\n\n";

    std::cout << "Punctuations:" << "\n";
    for (auto &punc : *punctuations) {
        std::cout << punc << std::endl;
    }
}

std::unordered_map<std::string, std::pair<std::string, int>> *Rules::getRegularExpressionsMap() const {
    return new std::unordered_map<std::string, std::pair<std::string, int>>(*regular_expressions_map);
}

std::unordered_map<std::string, std::pair<std::string, int>> *Rules::getRegularDefinitionsMap() const {
    return new std::unordered_map<std::string, std::pair<std::string, int>>(*regular_definitions_map);
}

std::vector<Token*> *Rules::getRegularExpressions() const {
    return regular_expressions_tokens_vector;
}

std::vector<Token*> *Rules::getRegularDefinitions() const {
    return regular_definitions_tokens_vector;
}

std::vector<std::string> *Rules::getKeywords() const {
    return keywords;
}

std::vector<std::string> *Rules::getPunctuations() const {
    return punctuations;
}

void Rules::setRegularExpressionsTokensVector(std::vector<Token*> *regularExpressionsTokensVector) {
    regular_expressions_tokens_vector = new std::vector<Token*>(*regularExpressionsTokensVector);
}

void Rules::setRegularDefinitionsTokensVector(std::vector<Token*> *regularDefinitionsTokensVector) {
    regular_definitions_tokens_vector = regularDefinitionsTokensVector;
}
