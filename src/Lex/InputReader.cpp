#include "Lex/InputReader.h"
#include "Lex/Utilities.h"

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
    readFile(file);
    Utilities::fixConcat(rules, non_terminal_symbols);
    rules->setRegularDefinitionsTokensVector(Utilities::convertMapToVector(rules->getRegularDefinitionsMap()));
    rules->setRegularExpressionsTokensVector(Utilities::convertMapToVector(rules->getRegularExpressionsMap()));
}

void InputReader::readTemplate(std::ifstream *file) {
    std::string line;
    int line_number = 0;
    while (getline(*file, line)) {
        line_number++;
        parseLine(line);
    }
}

void InputReader::parseLine(std::string &line) {
    RuleType line_type = checkType(&line);
    buildRule(line, line_type);
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

void InputReader::buildRule(std::string &pString, RuleType type) {
    if (type == RuleType::REGULAR_DEFINITION || type == RuleType::REGULAR_EXPRESSION)
        addRegularDefinitionOrExpression(pString, type);
    else if (type == RuleType::KEYWORDS)
        addKeyword(pString);
    else if (type == RuleType::PUNCTUATION)
        addPunctuation(pString);
    else
        return;
}

void InputReader::addPunctuation(std::string &pString) {
    std::string expression = pString.substr(1, pString.length() - 2);
    addPunctuations(expression);
}

void InputReader::addKeyword(std::string &pString) {
    std::string expression = pString.substr(1, pString.length() - 2);
    std::string delimiter = " ";
    addKeywords(expression, delimiter);
}

void InputReader::addRegularDefinitionOrExpression(std::string &pString, RuleType type) {
    char c = type == RuleType::REGULAR_DEFINITION ? '=' : ':';

    std::string name = pString.substr(0, pString.find(c));
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());

    std::string expression = pString.substr(pString.find(c) + 1);

    non_terminal_symbols->insert(name);
    rules->addRule(type, expression, name);
}

void InputReader::addKeywords(std::string &string, std::string &delimiter) {
    std::string::size_type pos;
    std::string::size_type prev = 0;
    while ((pos = string.find(delimiter, prev)) != std::string::npos) {
        std::string keyword = string.substr(prev, pos - prev);
        rules->addRule(RuleType::KEYWORDS, keyword);
        prev = pos + 1;
    }
    std::string keyword = string.substr(prev, pos - prev);
    rules->addRule(RuleType::KEYWORDS, keyword);
}

void InputReader::addPunctuations(std::string pString) {
    std::string::size_type pos = 0;
    std::string::size_type pString_length = pString.length();
    while (pos < pString_length) {
        if ((pString)[pos] == ' ') {
            pos++;
        } else if ((pString)[pos] == '\\') {
            std::string punctuation;
//            punctuation += pString[pos];
            punctuation += pString[pos + 1];
            rules->addRule(RuleType::PUNCTUATION, punctuation);
            pos += 2;
        } else {
            std::string punctuation;
            punctuation += pString[pos];
            rules->addRule(RuleType::PUNCTUATION, punctuation);
            pos++;
        }
    }
}
