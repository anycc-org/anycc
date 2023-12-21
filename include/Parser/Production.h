

#pragma once
#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <string>
#include <vector>

struct Production {
    Production() = default;

    std::string nonTerminal;
    std::vector<std::vector<std::string>> productions;
};

#endif // PRODUCTION_H
