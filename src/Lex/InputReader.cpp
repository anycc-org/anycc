#include <Lex/InputReader.h>

InputReader::~InputReader() = default;

InputReader::InputReader(std::string *rules_file_name, Rules *rules) {
    this->rules = rules;
    auto *file = new std::ifstream(*rules_file_name);
    parse_file(file);
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
    } else {
        std::cout << "Unable to open file";
    }
}

RuleType InputReader::check_type(std::string *basicString) {
    std::regex regular_expression_regex("[\\w\\d\\s]+:.+");
    std::regex regular_definition_regex("[\\w\\d\\s]+=.+");
    std::regex punctuation_regex("\\[.+\\]");
    std::regex keywords_regex("\\{.+\\}");

    if (std::regex_match(*basicString, regular_expression_regex)) {
        return RuleType::REGULAR_EXPRESSION;
    } else if (std::regex_match(*basicString, regular_definition_regex)) {
        return RuleType::REGULAR_DEFINITION;
    } else if (std::regex_match(*basicString, keywords_regex)) {
        return RuleType::KEYWORDS;
    } else if (std::regex_match(*basicString, punctuation_regex)) {
        return RuleType::PUNCTUATION;
    }

    return RuleType::NONE;
}

void InputReader::build_rule(RuleType type, std::string *pString) {
    std::string name;
    std::string expression;
    switch (type) {
        case RuleType::REGULAR_EXPRESSION:
            name = pString->substr(0, pString->find(':'));
            expression = pString->substr(pString->find(':') + 1, pString->length());
            rules->add_rule(RuleType::REGULAR_EXPRESSION, &name, &expression);
            break;
        case RuleType::REGULAR_DEFINITION:
            name = pString->substr(0, pString->find('='));
            expression = pString->substr(pString->find('=') + 1, pString->length());
            rules->add_rule(RuleType::REGULAR_DEFINITION, &name, &expression);
            break;
        case RuleType::KEYWORDS:
            expression = pString->substr(1, pString->length() - 2);
            split_string(&expression, new std::string(" "));
            break;
        case RuleType::PUNCTUATION:
            expression = pString->substr(1, pString->length() - 2);
            split_punctuation(&expression);
            break;
        default:
            break;
    }
}

void InputReader::split_string(std::string *string, std::string *delimiter) {
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

void InputReader::split_punctuation(std::string *pString) {
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