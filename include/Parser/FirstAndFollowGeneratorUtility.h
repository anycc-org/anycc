//
// Created by abdel on 01/12/2023.
//

#pragma once

#include "Parser/Production.h"
#include <set>
#include <string>
#include <vector>
using namespace std;
struct SubstringInfo {
    int start;
    int end;
};
inline bool compareSubstringInfo(const SubstringInfo& a, const SubstringInfo& b) {
    return a.start < b.start;
}

vector<SubstringInfo> findAllLongestSubstringIndices(const string& input, const set<string>& substrings);

bool isNT(const string& s,set<std::string>& nonTerminals);

set<string> collectNonTerminals(const vector<Production>& grammar);
string getLongestUpperCaseSequence(const string& str);