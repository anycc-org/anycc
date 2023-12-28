
#include "Parser/FirstAndFollowGeneratorUtility.h"
#include <algorithm>
#include <fstream>

std::vector<SubstringInfo>
findAllLongestSubstringIndices(const std::string &input, const std::set<std::string> &substrings) {
    std::vector<SubstringInfo> substringInfoVec;

    for (const std::string &substring: substrings) {
        size_t pos = input.find(substring);
        while (pos != std::string::npos) {
            int endIndex = static_cast<int>(pos + substring.length());
            bool found = false;

            for (auto &info: substringInfoVec) {
                if (info.start == static_cast<int>(pos) && endIndex > info.end) {
                    info.end = endIndex;
                    found = true;
                    break;
                }
            }

            if (!found) {
                substringInfoVec.push_back({static_cast<int>(pos), endIndex});
            }

            pos = input.find(substring, pos + 1);  // Move to the next occurrence
        }
    }
    std::sort(substringInfoVec.begin(), substringInfoVec.end(), compareSubstringInfo);

    return substringInfoVec;
}

bool isNT(const std::string &s, std::set<std::string> &nonTerminals) {
    return nonTerminals.find(s) != nonTerminals.end();
}

std::set<std::string> collectNonTerminals(const std::vector<Production> &grammar) {
    std::set<std::string> nonTerminals;
    for (const Production &rule: grammar) {
        nonTerminals.insert(rule.nonTerminal);
    }
    return nonTerminals;
}

std::string getLongestUpperCaseSequence(const std::string &str) {
    std::string currentSequence;
    std::string longestSequence;

    for (char ch: str) {
        if (isupper(ch)) {
            currentSequence += ch;
        } else {
            // Check if the current sequence is longer than the longest
            if (currentSequence.length() > longestSequence.length()) {
                longestSequence = currentSequence;
            }
            // Reset the current sequence
            currentSequence.clear();
        }
    }

    // Check if the last sequence is longer than the longest
    if (currentSequence.length() > longestSequence.length()) {
        longestSequence = currentSequence;
    }

    return longestSequence;
}
