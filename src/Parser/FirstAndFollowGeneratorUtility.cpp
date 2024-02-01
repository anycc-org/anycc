
#include "Parser/FirstAndFollowGeneratorUtility.h"
#include <algorithm>
#include <fstream>

std::set<std::string> collectNonTerminals(const std::vector<Production> &grammar) {
    std::set<std::string> nonTerminals;
    for (const Production &rule: grammar) {
        nonTerminals.insert(rule.nonTerminal);
    }
    return nonTerminals;
}
