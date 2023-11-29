#pragma once
#include <vector>
#include <string>
#include <stack>
#include "NFA.h"

class NFAGenerator {
public:
    NFAGenerator();

    NFA* buildNFA(const std::unordered_map<std::string, std::string>& regexMap,
                         const std::unordered_map<std::string, std::string>& regexDefMap);
    NFA* regexToNFA(const std::string& regex);
    NFA* combineNFAs(const std::vector<NFA*>& nfas);

    ~NFAGenerator();

    void processOperator(char op, std::stack<NFA*>& nfaStack);
private:
    std::unordered_map<std::string, NFA*> regexToNFAMap;
    int precedence(char op);

};