#include "utilities.h"

std::vector<SubstringInfo>
utilities::findAllLongestSubstringIndices(std::string *input, std::set<std::string> *substrings) {
    std::vector<SubstringInfo> substringInfoVec;

    for (const auto &substring: *substrings) {
        size_t pos = input->find(substring);
        while (pos != std::string::npos) {
            int endIndex = static_cast<int>(pos + substring.length());

            if ((endIndex < input->length()) && ((*input)[endIndex] == '+' || (*input)[endIndex] == '*'))
                endIndex++;

            bool found = false;

            for (auto &info: substringInfoVec) {
                if (info.start == static_cast<int>(pos) && endIndex > info.end) {
                    info.end = endIndex;
                    found = true;
                    break;
                }
            }

            if (!found)
                substringInfoVec.push_back({static_cast<int>(pos), endIndex});

            pos = input->find(substring, pos + 1);
        }
    }
    sort(substringInfoVec.begin(), substringInfoVec.end(), compareSubstringInfo);

    return substringInfoVec;
}

void utilities::fixSpaces(Rules *rules, std::set<std::string> *non_terminal_symbols) {
    auto regular_expressions = rules->getRegularExpressionsMap();

    fixSpacesGivenType(regular_expressions, rules, non_terminal_symbols, RuleType::REGULAR_EXPRESSION);

    auto regular_definitions = rules->getRegularDefinitionsMap();

    fixSpacesGivenType(regular_definitions, rules, non_terminal_symbols, RuleType::REGULAR_DEFINITION);
}

void utilities::fixSpacesGivenType(std::unordered_map<std::string, std::pair<std::string, int>> regular_rules, Rules *rules,
                                   std::set<std::string> *non_terminal_symbols, RuleType type) {
    for (auto &re: regular_rules) {
        auto *expression = new std::string(re.second.first);
        auto *name = new std::string(re.first);
        int offset = 0;
        std::vector<SubstringInfo> substringInfoVec = findAllLongestSubstringIndices(expression, non_terminal_symbols);

        for (int i = 0; i < substringInfoVec.size(); i++)
            offset = detectConcatThenAddSpaces(expression, substringInfoVec, offset, i);

        addSpaceAfterAndBeforeBraces(expression);

        rules->addRule(type, name, expression);
    }
}

int
utilities::detectConcatThenAddSpaces(std::string *expression, const std::vector<SubstringInfo> &substringInfoVec,
                                     int offset, int i) {

    int startIdx = substringInfoVec[i].start;
    int endIdx = substringInfoVec[i].end;

    if (startIdx != 0 && (*expression)[startIdx + offset - 1] != '(' &&
        (*expression)[startIdx + offset - 1] != ')' &&
        (*expression)[startIdx + offset - 1] != '|' && (i == 0 || substringInfoVec[i - 1].end + offset !=
                                                                  startIdx + offset)) {
        expression->insert(startIdx + offset, " ");
        offset++;
    }

    if ((*expression)[endIdx + offset] != '(' && (*expression)[endIdx + offset] != ')' &&
        (*expression)[endIdx + offset] != '|') {
        expression->insert(endIdx + offset, " ");
        offset++;
    }
    return offset;
}

void utilities::addSpaceAfterAndBeforeBraces(std::string *expression) {
    for (int i = 0; i < expression->length() - 1; i++) {
        if ((*expression)[i] == ')' && (*expression)[i + 1] != ' ' && (*expression)[i + 1] != '|' &&
            (*expression)[i + 1] != '*' && (*expression)[i + 1] != '+') {
            expression->insert(i + 1, " ");
            i++;
        } else if (i + 2 < expression->length() && (*expression)[i] == ')' &&
                   ((*expression)[i + 1] == '*' || (*expression)[i + 1] == '+')
                   && (*expression)[i + 2] != ' ' && (*expression)[i + 2] != '|') {
            expression->insert(i + 2, " ");
            i++;
        } else if (i != 0 && (*expression)[i] == '(' && (*expression)[i - 1] != ' ' && (*expression)[i - 1] != '|') {
            expression->insert(i, " ");
            i++;
        }
    }
}

std::vector<Token*> utilities::convertMapToVector(const std::unordered_map<std::string, std::pair<std::string, int>>& map) {
    auto vector = std::vector<Token*>(map.size());

    for (auto &item: map) {
        auto *key = new std::string(item.first);
        auto *value = new std::string(item.second.first);
        auto *token = new Token(key, value);
        vector[item.second.second] = token;
    }

    return vector;
}

void utilities::deleteVectorOfTokens(std::vector<Token*> *vector) {
    for (auto token : *vector) {
        delete token;
    }
}
