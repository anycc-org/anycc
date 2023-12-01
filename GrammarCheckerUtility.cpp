//
// Created by abdel on 01/12/2023.
//
#include "GrammarCheckerUtility.h"
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

    return substringInfoVec;
}
bool isNT(const string& s,set<std::string>& nonTerminals) {
    return nonTerminals.find(s) != nonTerminals.end();
}

set<string>  collectNonTerminals(const vector<Production>& grammar) {
    set<string> nonTerminals;
    for (const Production& rule : grammar) {
        nonTerminals.insert(rule.nonTerminal);
    }
    return nonTerminals;
}