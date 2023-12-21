#include "Parser/LeftRecursionRemover.h"


bool LeftRecursionRemover::isImmediateLR(const std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, std::string non_terminal) {
    return false;
}

bool LeftRecursionRemover::removeImmediateLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, std::string non_terminal) {
    return false;
}

bool LeftRecursionRemover::substituteRHS(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, std::string lhs_non_terminal, std::string rhs_non_terminal) {
    return false;
}

bool LeftRecursionRemover::removeLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar) {
    return false;
}