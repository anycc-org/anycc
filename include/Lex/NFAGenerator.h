#pragma once
#include <vector>
#include <string>
#include <stack>
#include <map>
#include "NFA.h"
#include "Token.h"

class NFAGenerator {
public:
    NFAGenerator();

    NFA* buildNFA(const std::vector<Token*>& regexMap,
                         const std::vector<Token*>& regexDefMap,
                         const std::vector<std::string>& keywords,
                         const std::vector<std::string>& punctuations);
    NFA* regexToNFA(const std::string& regex);
    NFA* combineNFAs(std::vector<NFA*>& nfas);

    ~NFAGenerator();

    void processOperator(char op, std::stack<NFA*>& nfaStack);
private:
    std::unordered_map<std::string, NFA*> regexToNFAMap;
    int precedence(char op) const;
    bool isOperator(char op) const;
};