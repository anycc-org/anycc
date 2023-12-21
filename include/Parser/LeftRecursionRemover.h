#ifndef LEFT_RECUSRION_REMOVER_H
#define LEFT_RECUSRION_REMOVER_H

#include <vector>
#include <string>
#include <unordered_map>


class LeftRecursionRemover {
public:
    
    static bool isImmediateLR(const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string non_terminal);

    static bool removeImmediateLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string non_terminal);

    static bool substituteRHS(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string lhs_non_terminal, std::string rhs_non_terminal);

    static bool removeLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar);
    
};


#endif // LEFT_RECUSRION_REMOVER_H