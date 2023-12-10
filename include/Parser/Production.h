//
// Created by abdel on 30/11/2023.
//

#pragma once
#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <string>
#include <vector>
using namespace std;

struct Production {
    string nonTerminal;
    vector<vector<string>> productions;
};

#endif // PRODUCTION_H
