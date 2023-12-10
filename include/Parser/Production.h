//
// Created by abdel on 30/11/2023.
//

#pragma once
#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <string>
#include <vector>
struct Production {
    std::string nonTerminal;
    std::vector<std::vector<std::string>> productions;
};

#endif // PRODUCTION_H
