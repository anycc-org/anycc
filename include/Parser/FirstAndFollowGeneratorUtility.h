#ifndef FIRSTANDFOLLOWGENERATORUTILITY_H
#define FIRSTANDFOLLOWGENERATORUTILITY_H

#include "Parser/Production.h"
#include "SubstringInfo.h"
#include <set>
#include <string>
#include <vector>

std::set<std::string> collectNonTerminals(const std::vector<Production> &grammar);

#endif // FIRSTANDFOLLOWGENERATORUTILITY_H
