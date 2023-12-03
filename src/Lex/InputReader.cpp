#include <Lex/InputReader.h>
#include "utilities.h"

InputReader::~InputReader() {
    delete non_terminal_symbols;
}

InputReader::InputReader(std::string *rules_file_name, Rules *rules) {
    this->rules = rules;
    this->non_terminal_symbols = new std::set<std::string>();
    auto *file = new std::ifstream(*rules_file_name);

    build_rules(file);
}

void InputReader::build_rules(std::ifstream *file) {
    parse_file(file);
    utilities::fix_spaces(rules, non_terminal_symbols);
    rules->setRegularDefinitionsTokensVector(utilities::convert_map_to_vector(rules->getRegularDefinitionsMap()));
    rules->setRegularExpressionsTokensVector(utilities::convert_map_to_vector(rules->getRegularExpressionsMap()));
}

void InputReader::parse_file(std::ifstream *file) {
    std::string line;
    if (file->is_open()) {
        while (getline(*file, line)) {
            std::cout << line << "\n";
            RuleType line_type = check_type(&line);
            build_rule(line_type, &line);
        }
        file->close();
        delete file;
    } else {
        std::cout << "Unable to open file";
    }
}

RuleType InputReader::check_type(std::string *basicString) {
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

void InputReader::build_rule(RuleType type, std::string *pString) {
    if (type == RuleType::REGULAR_DEFINITION || type == RuleType::REGULAR_EXPRESSION)
        add_regular_definition_or_expression(pString, type);
    else if (type == RuleType::KEYWORDS)
        add_keyword(pString);
    else if (type == RuleType::PUNCTUATION)
        add_punctuation(pString);
    else
        std::cout << "Invalid rule: " << *pString << '\n';
}

void InputReader::add_punctuation(const std::string *pString) {
    std::string expression = pString->substr(1, pString->length() - 2);
    add_punctuations(&expression);
}

void InputReader::add_keyword(const std::string *pString) {
    std::string expression = pString->substr(1, pString->length() - 2);
    add_keywords(&expression, new std::string(" "));
}

void InputReader::add_regular_definition_or_expression(const std::string *pString, RuleType type) {
    char c = type == RuleType::REGULAR_DEFINITION ? '=' : ':';

    auto *name = new std::string(pString->substr(0, pString->find(c)));
    name->erase(std::remove(name->begin(), name->end(), ' '), name->end());

    auto *expression = new std::string(pString->substr(pString->find(c) + 1));
    expression->erase(std::remove(expression->begin(), expression->end(), ' '), expression->end());

    non_terminal_symbols->insert(*name);
    rules->add_rule(type, name, expression);
}

void InputReader::add_keywords(std::string *string, std::string *delimiter) {
    std::string::size_type pos;
    std::string::size_type prev = 0;
    while ((pos = string->find(*delimiter, prev)) != std::string::npos) {
        auto *keyword = new std::string(string->substr(prev, pos - prev));
        rules->add_rule(RuleType::KEYWORDS, nullptr, keyword);
        prev = pos + 1;
    }
    auto *keyword = new std::string(string->substr(prev, pos - prev));
    rules->add_rule(RuleType::KEYWORDS, nullptr, keyword);
}

void InputReader::add_punctuations(std::string *pString) {
    std::string::size_type pos = 0;
    std::string::size_type pString_length = pString->length();
    while (pos < pString_length) {
        if ((*pString)[pos] == ' ') {
            pos++;
        } else if ((*pString)[pos] == '\\') {
            auto *punctuation = new std::string();
            *punctuation += (*pString)[pos];
            *punctuation += (*pString)[pos + 1];
            rules->add_rule(RuleType::PUNCTUATION, nullptr, punctuation);
            pos += 2;
        } else {
            auto *punctuation = new std::string();
            *punctuation += (*pString)[pos];
            rules->add_rule(RuleType::PUNCTUATION, nullptr, punctuation);
            pos++;
        }
    }
}

