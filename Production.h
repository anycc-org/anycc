//
// Created by abdel on 30/11/2023.
//

#pragma once
#ifndef ANYCC_PRODUCTION_H
#define ANYCC_PRODUCTION_H



#include <vector>
#include <string>

struct Production {
    std::string nonTerminal;
    std::vector<std::vector<std::string>> productions;
};