#ifndef LEFT_RECUSRION_REMOVER_H
#define LEFT_RECUSRION_REMOVER_H

#include <vector>
#include <string>
#include <unordered_map>


class LeftRecursionRemover {
public:

    static std::unordered_map<std::string, std::vector<std::vector<std::string>>> removeLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar);

private:
    static bool isImmediateLR(const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string non_terminal);

    static void removeImmediateLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string non_terminal);

    static void substituteRHS(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string lhs_non_terminal, std::string rhs_non_terminal);
};


#endif // LEFT_RECUSRION_REMOVER_H