//
// Created by abdel on 01/12/2023.
//
#include "Parser/FirstAndFollowGeneratorUtility.h"
#include <algorithm>
vector<SubstringInfo> findAllLongestSubstringIndices(const string& input, const set<string>& substrings) {
    vector<SubstringInfo> substringInfoVec;

    for (const string& substring : substrings) {
        size_t pos = input.find(substring);
        while (pos != string::npos) {
            int endIndex = static_cast<int>(pos + substring.length());
            bool found = false;

            for (auto& info : substringInfoVec) {
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
    sort(substringInfoVec.begin(), substringInfoVec.end(), compareSubstringInfo);

    return substringInfoVec;
}
bool isNT(const string& s,set<string>& nonTerminals) {
    return nonTerminals.find(s) != nonTerminals.end();
}

set<string>  collectNonTerminals(const vector<Production>& grammar) {
    set<string> nonTerminals;
    for (const Production& rule : grammar) {
        nonTerminals.insert(rule.nonTerminal);
    }
    return nonTerminals;
}
string getLongestUpperCaseSequence(const string& str) {
     string currentSequence;
     string longestSequence;

    for (char ch : str) {
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
