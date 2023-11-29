#pragma once
#include <vector>
#include <string>
#include <stack>
#include "NFA.h"

class NFAGenerator {
public:
    NFAGenerator();

    static NFA* regexToNFA(const std::string& regex);
    static NFA* combineNFAs(const std::vector<NFA*>& nfas);

    ~NFAGenerator();

    static void processOperator(char op, std::stack<NFA*>& nfaStack);
private:
    static int precedence(char op);

};