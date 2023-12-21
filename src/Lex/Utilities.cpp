#include "Utilities.h"
#include "Operator.h"
#include <regex>

std::string *Utilities::cleanRegex(std::string *input) {
    // Remove spaces before + or *
    std::regex spacesBeforePlus("\\s*\\+");
    *input = std::regex_replace(*input, spacesBeforePlus, "+");

    std::regex spacesBeforeStar("\\s*\\*");
    *input = std::regex_replace(*input, spacesBeforeStar, "*");

    // Remove spaces before and after -
    std::regex spacesBeforeMinus("\\s*\\-\\s*");
    *input = std::regex_replace(*input, spacesBeforeMinus, "-");

    // Remove spaces around |
    std::regex spacesAroundPipe("\\s*\\|\\s*");
    *input = std::regex_replace(*input, spacesAroundPipe, "|");

    // Remove multiple spaces
    std::regex multipleSpaces("\\s{2,}");
    *input = std::regex_replace(*input, multipleSpaces, " ");

    // Trim spaces at the start and end
    *input = std::regex_replace(*input, std::regex("^\\s+|\\s+$"), "");

    // remove space before and after braces
    *input = std::regex_replace(*input, std::regex("\\s*\\(\\s*"), "(");
    *input = std::regex_replace(*input, std::regex("\\s*\\)\\s*"), ")");

    return input;
}

std::vector<SubstringInfo>
Utilities::findAllLongestSubstringIndices(std::string *input, std::set<std::string> *substrings) {
    std::vector<SubstringInfo> substringInfoVec;

    for (const auto &substring: *substrings) {
        size_t pos = input->find(substring);
        while (pos != std::string::npos) {
            int endIndex = static_cast<int>(pos + substring.length());

            if ((endIndex < input->length()) && ((*input)[endIndex] == (char) Operator::PLUS ||
                                                 (*input)[endIndex] == (char) Operator::STAR))
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

void Utilities::fixConcat(Rules *rules, std::set<std::string> *non_terminal_symbols) {
    auto regular_expressions = rules->getRegularExpressionsMap();

    fixConcatGivenType(regular_expressions, rules, non_terminal_symbols, RuleType::REGULAR_EXPRESSION);

    auto regular_definitions = rules->getRegularDefinitionsMap();

    fixConcatGivenType(regular_definitions, rules, non_terminal_symbols, RuleType::REGULAR_DEFINITION);
}

void
Utilities::fixConcatGivenType(const std::unordered_map<std::string, std::pair<std::string, int>> &regular_rules,
                              Rules *rules, std::set<std::string> *non_terminal_symbols, RuleType type) {
    for (auto &re: regular_rules) {
        auto *expression = new std::string(re.second.first);
        expression = cleanRegex(expression);
        auto *name = new std::string(re.first);
        int offset = 0;
        std::vector<SubstringInfo> substringInfoVec = findAllLongestSubstringIndices(expression, non_terminal_symbols);

        for (int i = 0; i < substringInfoVec.size(); i++)
            offset = detectConcatThenAddSpaces(expression, substringInfoVec, offset, i);

        addSpaceAfterAndBeforeBraces(expression);

        rules->addRule(type, *expression, *name);
    }
}

int
Utilities::detectConcatThenAddSpaces(std::string *expression, const std::vector<SubstringInfo> &substringInfoVec,
                                     int offset, int i) {
    std::string concat_operator = std::string(1, (char) Operator::CONCAT);
    int startIdx = substringInfoVec[i].start;
    int endIdx = substringInfoVec[i].end;

    if (startIdx != 0 && !isConcat(expression, startIdx + offset - 1) &&
        !isOpenBrace(expression, startIdx + offset - 1) &&
        !isCloseBrace(expression, startIdx + offset - 1) &&
        !isOr(expression, startIdx + offset - 1) &&
        (i == 0 || substringInfoVec[i - 1].end + offset != startIdx + offset)) {
        expression->insert(startIdx + offset, std::string(1, (char) Operator::CONCAT));
        offset++;
    }

    if (endIdx + offset < expression->length() &&
        !isConcat(expression, endIdx + offset) &&
        !isOpenBrace(expression, endIdx + offset) &&
        !isCloseBrace(expression, endIdx + offset) &&
        !isOr(expression, endIdx + offset)) {
        expression->insert(endIdx + offset, concat_operator);
        offset++;
    }
    return offset;
}

void Utilities::addSpaceAfterAndBeforeBraces(std::string *expression) {
    std::string concat_operator = std::string(1, (char) Operator::CONCAT);
    for (int i = 0; i < expression->length() - 1; i++) {
        if (isCloseBrace(expression, i) &&
            !isConcat(expression, i + 1) &&
            !isOr(expression, i + 1) &&
            !isKleeneClosure(expression, i + 1) &&
            !isPositiveClosure(expression, i + 1)) {
            expression->insert(i + 1, concat_operator);
            i++;
        } else if (i + 2 < expression->length() && isCloseBrace(expression, i) &&
                   (isKleeneClosure(expression, i + 1) || isPositiveClosure(expression, i + 1))
                   && !isConcat(expression, i + 2) && !isOr(expression, i + 2)) {
            expression->insert(i + 2, concat_operator);
            i++;
        } else if (i != 0 && isOpenBrace(expression, i) &&
                   !isConcat(expression, i - 1) &&
                   !isOr(expression, i - 1)) {
            expression->insert(i, concat_operator);
            i++;
        }
    }
}

bool Utilities::isOpenBrace(const std::string *expression, int i) {
    return (*expression)[i] == (char) Operator::OPEN_BRACE;
}

bool Utilities::isPositiveClosure(const std::string *expression, int i) {
    return (*expression)[i] == (char) Operator::PLUS;
}

bool Utilities::isKleeneClosure(const std::string *expression, int i) {
    return (*expression)[i] == (char) Operator::STAR;
}

bool Utilities::isOr(const std::string *expression, int i) { return (*expression)[i] == (char) Operator::OR; }

bool Utilities::isConcat(const std::string *expression, int i) {
    return (*expression)[i] == (char) Operator::CONCAT;
}

bool Utilities::isCloseBrace(const std::string *expression, int i) {
    return (*expression)[i] == (char) Operator::CLOSE_BRACE;
}

std::vector<Token *>
Utilities::convertMapToVector(const std::unordered_map<std::string, std::pair<std::string, int>> &map) {
    auto vector = std::vector<Token *>(map.size());

    for (auto &item: map) {
        auto *key = new std::string(item.first);
        auto *value = new std::string(item.second.first);
        auto *token = new Token(key, value);
        vector[item.second.second] = token;
    }

    return vector;
}

void Utilities::deleteVectorOfTokens(std::vector<Token *> *vector) {
    for (auto token: *vector) {
        delete token;
    }
}


std::unordered_map<std::string, std::vector<std::vector<std::string>>> Utilities::parseCFGInput(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar;

    if (inputFile.is_open()) {
        std::string line;
        std::string currentNonTerminal;

        while (std::getline(inputFile, line)) {
            // Skip comments and empty lines
            if (line.empty())
                continue;

            std::vector<std::string> production;
            if(line[0] == '#'){// new rule
             // Find the position of = in the line
              std::regex spacesBeforeArrow("\\s*\\=");
              line = std::regex_replace(line, spacesBeforeArrow, "=");
              size_t arrowPos = line.find("="); // first occurence
              if (arrowPos != std::string::npos) {
                  // Extract the non-terminal before ::= as the current non-terminal
                  currentNonTerminal = line.substr(2, arrowPos-2); //skip '#' index 0 and ' ' index 1

                  // Tokenize the RHS of the production rule using '|'
                  line = line.substr(arrowPos + 1); // Move past "="
                  std::istringstream ss(line);
                  std::string token;

                  while (ss >> token) {
                      if(token == "|" && production.empty()) {
                        continue ;
                      }
                      if (token == "|" && !production.empty()) {
                        // Start a new production for the same non-terminal
                        grammar[currentNonTerminal].push_back(production);
                        production.clear();
                      } else {
                        token = std::regex_replace(token, std::regex("'"), "");
                        production.push_back(token);
                      }
                  }
              }
            }else{
                  std::istringstream ss(line);
                  std::string token;

                  while (ss >> token) {
                      if(token == "|" && production.empty()) {
                        continue ;
                      }
                      if (token == "|") {
                        // Start a new production for the same non-terminal
                        grammar[currentNonTerminal].push_back(production);
                        production.clear();
                      } else {
                        token = std::regex_replace(token, std::regex("'"), "");
                        production.push_back(token);
                      }
                  }
            }
                // Add the last production to the grammar
              grammar[currentNonTerminal].push_back(production);
        }

        inputFile.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return grammar;
}