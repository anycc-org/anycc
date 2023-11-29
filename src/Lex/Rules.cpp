#include <iostream>
#include "Rules.h"

Rules::Rules() {
    regular_expressions = new std::unordered_map<std::string, std::string>();
    regular_definitions = new std::unordered_map<std::string, std::string>();
    keywords = new std::vector<std::string>();
    punctuations = new std::vector<std::string>();
}

Rules::~Rules() {
    delete regular_expressions;
    delete regular_definitions;
    delete keywords;
    delete punctuations;
}

void Rules::add_punctuation(std::string *punctuation) {
    this->punctuations->push_back(*punctuation);
}

void Rules::add_regular_definition(std::string *name, std::string *definition) {
    this->regular_definitions->insert(std::make_pair(*name, *definition));
}

void Rules::add_regular_expression(std::string *name, std::string *expression) {
    this->regular_expressions->insert(std::make_pair(*name, *expression));
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
    for (auto &re : *regular_expressions) {
        std::cout << re.first << " : " << re.second << std::endl;
    }
    std::cout << "\n\n";

    std::cout << "Regular Definitions:" << "\n";
    for (auto &rd : *regular_definitions) {
        std::cout << rd.first << " : " << rd.second << std::endl;
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
