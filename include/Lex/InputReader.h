#pragma once

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <set>
#include "Rules.h"
#include "RuleType.h"
#include "FileReader.h"

struct SubstringInfo {
    int start;
    int end;
};

inline bool compareSubstringInfo(const SubstringInfo &a, const SubstringInfo &b) {
    return a.start < b.start;
}

class InputReader : public FileReader {
public:
    explicit InputReader(std::string *rules_file_name, Rules *rules);

    void readTemplate(std::ifstream *file) override;

    /**
     * @brief Parse the given line
     * @param line The line to be parsed
     */
    void parseLine(std::string &line);

    ~InputReader();

private:
    std::set<std::string> *non_terminal_symbols;
    Rules *rules;

    /**
     * @brief Check the type of the rule
     * @param basicString The string to be checked
     * @return The type of the rule
     */
    static RuleType checkType(std::string *basicString);

    /**
     * @brief Build the given rule
     * @param pString The string to be built has format: <name> = <expression> or <name> : <expression> or {...} or [...]
     * @param type The type of the rule
     */
    void buildRule(std::string &pString, RuleType type);

    /**
     * @brief Add keywords to the rules object from the given string
     * @param string on the format: {<keyword> <keyword> ...}
     * @param delimiter The delimiter between keywords
     */
    void addKeywords(std::string &string, std::string &delimiter);

    /**
     * @brief Add punctuations to the rules object from the given string
     * @param pString on the format: [<punctuation> <punctuation> ...]
     */
    void addPunctuations(std::string pString);

    /**
     * @brief Add regular definition or expression to the rules object from the given string
     * @param pString on the format: <name> = <expression> or <name> : <expression>
     * @param type The type of the rule (regular definition or expression)
     */
    void addRegularDefinitionOrExpression(std::string &pString, RuleType type);

    /**
     * @brief Add keyword to the rules object from the given string
     * @param pString on the format: <keyword>
     */
    void addKeyword(std::string &pString);

    /**
     * @brief Add punctuation to the rules object from the given string
     * @param pString on the format: <punctuation>
     */
    void addPunctuation(std::string &pString);

    /**
     * @brief Build the rules object from the given file
     * @param file
     */
    void buildRules(std::ifstream *file);
};