#include <Lex/InputReader.h>
#include "Utilities.h"

InputReader::InputReader(std::string *rules_file_name, Rules *rules) {
    this->rules = rules;
    this->non_terminal_symbols = new std::set<std::string>();
    auto *file = new std::ifstream(*rules_file_name);

    buildRules(file);
}

InputReader::~InputReader() {
    delete non_terminal_symbols;
}

void InputReader::buildRules(std::ifstream *file) {
    parseFile(file);
    Utilities::fixSpaces(rules, non_terminal_symbols);
    rules->setRegularDefinitionsTokensVector(Utilities::convertMapToVector(rules->getRegularDefinitionsMap()));
    rules->setRegularExpressionsTokensVector(Utilities::convertMapToVector(rules->getRegularExpressionsMap()));
}

void InputReader::parseFile(std::ifstream *file) {
    std::string line;
    if (file->is_open()) {
        while (getline(*file, line)) {
            std::cout << line << "\n";
            RuleType line_type = checkType(&line);
            buildRule(line_type, &line);
        }
        file->close();
        delete file;
    } else {
        std::cout << "Unable to open file";
    }
}

RuleType InputReader::checkType(std::string *basicString) {
    std::regex regular_expression_regex("[\\w\\d\\s]+:.+");
    std::regex regular_definition_regex("[\\w\\d\\s]+=.+");
    std::regex punctuation_regex("\\[.+\\]");
    std::regex keywords_regex("\\{.+\\}");

    if (std::regex_match(*basicString, regular_expression_regex))
        return RuleType::REGULAR_EXPRESSION;
    else if (std::regex_match(*basicString, regular_definition_regex))
        return RuleType::REGULAR_DEFINITION;
    else if (std::regex_match(*basicString, keywords_regex))
        return RuleType::KEYWORDS;
    else if (std::regex_match(*basicString, punctuation_regex))
        return RuleType::PUNCTUATION;

    return RuleType::NONE;
}

void InputReader::buildRule(RuleType type, std::string *pString) {
    if (type == RuleType::REGULAR_DEFINITION || type == RuleType::REGULAR_EXPRESSION)
        addRegularDefinitionOrExpression(pString, type);
    else if (type == RuleType::KEYWORDS)
        addKeyword(pString);
    else if (type == RuleType::PUNCTUATION)
        addPunctuation(pString);
    else
        std::cout << "Invalid rule: " << *pString << '\n';
}

void InputReader::addPunctuation(const std::string *pString) {
    auto *expression = new std::string(pString->substr(1, pString->length() - 2));
    addPunctuations(expression);

    delete expression;
}

void InputReader::addKeyword(const std::string *pString) {
    auto *expression = new std::string(pString->substr(1, pString->length() - 2));
    auto *delimiter = new std::string(" ");
    addKeywords(expression, delimiter);

    delete expression;
    delete delimiter;
}

void InputReader::addRegularDefinitionOrExpression(const std::string *pString, RuleType type) {
    char c = type == RuleType::REGULAR_DEFINITION ? '=' : ':';

    auto *name = new std::string(pString->substr(0, pString->find(c)));
    name->erase(std::remove(name->begin(), name->end(), ' '), name->end());

    auto *expression = new std::string(pString->substr(pString->find(c) + 1));
    expression->erase(std::remove(expression->begin(), expression->end(), ' '), expression->end());

    non_terminal_symbols->insert(*name);
    rules->addRule(type, name, expression);

    delete name;
    delete expression;
}

void InputReader::addKeywords(std::string *string, std::string *delimiter) {
    std::string::size_type pos;
    std::string::size_type prev = 0;
    while ((pos = string->find(*delimiter, prev)) != std::string::npos) {
        auto *keyword = new std::string(string->substr(prev, pos - prev));
        rules->addRule(RuleType::KEYWORDS, nullptr, keyword);
        prev = pos + 1;
        delete keyword;
    }
    auto *keyword = new std::string(string->substr(prev, pos - prev));
    rules->addRule(RuleType::KEYWORDS, nullptr, keyword);

    delete keyword;
}

void InputReader::addPunctuations(std::string *pString) {
    std::string::size_type pos = 0;
    std::string::size_type pString_length = pString->length();
    while (pos < pString_length) {
        if ((*pString)[pos] == ' ') {
            pos++;
        } else if ((*pString)[pos] == '\\') {
            auto *punctuation = new std::string();
            *punctuation += (*pString)[pos];
            *punctuation += (*pString)[pos + 1];
            rules->addRule(RuleType::PUNCTUATION, nullptr, punctuation);
            pos += 2;
            delete punctuation;
        } else {
            auto *punctuation = new std::string();
            *punctuation += (*pString)[pos];
            rules->addRule(RuleType::PUNCTUATION, nullptr, punctuation);
            pos++;
            delete punctuation;
        }
    }
}

