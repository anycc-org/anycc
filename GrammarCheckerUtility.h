//
// Created by abdel on 01/12/2023.
//

#pragma once

#include <vector>
#include <string>
#include <set>
#include "Production.h"
using namespace std;
struct SubstringInfo {
    int start;
    int end;
};

vector<SubstringInfo> findAllLongestSubstringIndices(const string& input, const set<string>& substrings);

bool isNT(const string& s,set<std::string>& nonTerminals);

set<string> collectNonTerminals(const vector<Production>& grammar);