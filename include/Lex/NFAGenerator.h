#pragma once

#include <vector>
#include <string>
#include <stack>
#include <map>
#include "Lex/NFA.h"
#include "Lex/Token.h"

class NFAGenerator {
public:
    NFAGenerator();

    NFA *buildNFA(const std::vector<Token *> &regexMap,
                  const std::vector<Token *> &regexDefMap,
                  const std::vector<std::string> &keywords,
                  const std::vector<std::string> &punctuations);

    NFA *regexToNFA(const std::string &regex);

    static NFA *combineNFAs(std::vector<NFA *> &nfas);

    ~NFAGenerator();

    static void processOperator(char op, std::stack<NFA *> &nfaStack);

private:
    std::unordered_map<std::string, NFA *> regexToNFAMap;

    static int precedence(char op);

    static bool isOperator(char op);
};