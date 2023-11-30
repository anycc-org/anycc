//
// Created by abdel on 30/11/2023.
//

#pragma once

#include <vector>
#include <string>

struct Production {
    std::string nonTerminal;
    std::vector<std::vector<std::string>> productions;
};