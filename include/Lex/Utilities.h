#ifndef ANYCC_UTILITIES_H
#define ANYCC_UTILITIES_H

#include "InputReader.h"
#include "Operator.h"
#include "RuleType.h"
#include <regex>
#include <vector>

class Utilities {
public:
    /**
     * @brief clean the given rule from extra spaces, spaces before and after braces, spaces before and after operators
     * @param input Rule to be cleaned
     * @return Cleaned rule
     */
    static std::string *cleanRegex(std::string *input);

    /**
     * @brief Find all the indices of the longest substrings in the given input
     * @param input The input to be searched
     * @param substrings The substrings to be searched for
     * @return A sorted vector of SubstringInfo, each SubstringInfo contains the start and end indices of the substring
     */
    static std::vector<SubstringInfo>
    findAllLongestSubstringIndices(std::string *input, std::set<std::string> *substrings);

    /**
     * @brief Fix the given rule by adding spaces before and after braces, operators around the definition of the rule given from the substringInfoVec
     * @param regular_rules The rules map to be fixed
     * @param rules The rules object to be modified
     * @param non_terminal_symbols non terminal symbols to detected and fixed if needed
     * @param type type of the rule
     */
    static void
    fixConcatGivenType(const std::unordered_map<std::string, std::pair<std::string, int>> &regular_rules, Rules *rules,
                       std::set<std::string> *non_terminal_symbols, RuleType type);

    /**
     * @brief Detect the non terminal symbols and add spaces before and after them if needed
     * @param expression The expression to be fixed
     * @param substringInfoVec The vector of SubstringInfo to be used to detect the non terminal symbols
     * @param offset The offset created after adding spaces before or after the non terminal symbols
     * @param i The index of the current SubstringInfo
     * @return The new offset after adding spaces before or after the non terminal symbols
     */
    static int
    detectConcatThenAddSpaces(std::string *expression, const std::vector<SubstringInfo> &substringInfoVec,
                              int offset, int i);

    /**
     * @brief Add spaces before and after the given braces if needed
     * @param expression The expression to be fixed
     */
    static void addSpaceAfterAndBeforeBraces(std::string *expression);

    /**
     * @brief Fix the concat operator in the given rules object by calling fixConcatGivenType for each type (regular expression and definition)
     * @param rules The rules object to be fixed
     * @param non_terminal_symbols The non terminal symbols to be detected and fixed if needed
     */
    static void fixConcat(Rules *rules, std::set<std::string> *non_terminal_symbols);

    /**
     * @brief Convert the given map to a vector of tokens
     * @param map The map to be converted
     * @return The vector of tokens
     */
    static std::vector<Token *>
    convertMapToVector(const std::unordered_map<std::string, std::pair<std::string, int>> &map);

    /**
     * @brief Delete the given vector of tokens
     * @param vector The vector of tokens to be deleted
     */
    static void deleteVectorOfTokens(std::vector<Token *> *vector);

    /**
     * @brief Check if the i-th character in expression is a close brace
     * @param expression The expression to be checked
     * @param i The index of the character to be checked
     * @return True if the i-th character in expression is a close brace, false otherwise
     */
    static bool isCloseBrace(const std::string *expression, int i);

    /**
     * @brief Check if the i-th character in expression is a concat operator
     * @param expression The expression to be checked
     * @param i The index of the character to be checked
     * @return True if the i-th character in expression is a concat operator, false otherwise
     */
    static bool isConcat(const std::string *expression, int i);

    /**
     * @brief Check if the i-th character in expression is an or operator
     * @param expression The expression to be checked
     * @param i The index of the character to be checked
     * @return True if the i-th character in expression is an or operator, false otherwise
     */
    static bool isOr(const std::string *expression, int i);

    /**
     * @brief Check if the i-th character in expression is a kleene closure operator
     * @param expression The expression to be checked
     * @param i The index of the character to be checked
     * @return True if the i-th character in expression is a kleene closure operator, false otherwise
     */
    static bool isKleeneClosure(const std::string *expression, int i);

    /**
     * @brief Check if the i-th character in expression is a positive closure operator
     * @param expression The expression to be checked
     * @param i The index of the character to be checked
     * @return True if the i-th character in expression is a positive closure operator, false otherwise
     */
    static bool isPositiveClosure(const std::string *expression, int i);

    /**
     * @brief Check if the i-th character in expression is an open brace
     * @param expression The expression to be checked
     * @param i The index of the character to be checked
     * @return True if the i-th character in expression is an open brace, false otherwise
     */
    static bool isOpenBrace(const std::string *expression, int i);
};

#endif //ANYCC_UTILITIES_H