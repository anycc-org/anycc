#include <Lex/NFAGenerator.h>
#include <stack>
#include <iostream>

NFAGenerator::NFAGenerator() = default;

NFAGenerator::~NFAGenerator() = default;

NFA *NFAGenerator::buildNFA(const std::unordered_map<std::string, std::string> &regexMap,
                            const std::unordered_map<std::string, std::string> &regexDefMap) {
    std::vector<NFA*> nfas;
    for (auto& regexDef : regexDefMap) {
        NFA* nfa = regexToNFA(regexDef.second);
        nfas.push_back(nfa);
        regexToNFAMap[regexDef.first] = nfa;
    }

    for (auto& regex : regexMap) {
        nfas.push_back(regexToNFA(regex.second));
    }

    return combineNFAs(nfas);
}

/**
 * @return NFA after regex evaluation (postfix)
 */
NFA* NFAGenerator::regexToNFA(const std::string& regex) {
    std::stack<NFA*> nfaStack;
    std::stack<char> operatorStack;

    for (char c : regex) { // l (l|d)*

        if (c == '(') {
            operatorStack.push('(');
        }
        else if (isalpha(c) || isdigit(c) || c == '.') {
            nfaStack.push(NFA::basicCharToNFA(c));
        }
        else if (c == ')') {
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                char op = operatorStack.top();
                operatorStack.pop();

                processOperator(op, nfaStack);
            }
            // Pop '(' from the operator stack
            if (!operatorStack.empty())
                operatorStack.pop();
        }
        else { // Operator encountered {*, +, ' ', |, -}
            while (!operatorStack.empty() && precedence(operatorStack.top()) >= precedence(c)) {
                char op = operatorStack.top();
                operatorStack.pop();

                processOperator(op, nfaStack);
            }
            operatorStack.push(c);
        }
    }

    while (!operatorStack.empty()) {
        char op = operatorStack.top();
        operatorStack.pop();

        processOperator(op, nfaStack);
    }

    // Top of the stack contains the final NFA
    std::cout << "nfa stack size: " << nfaStack.size() << std::endl;
    return nfaStack.top();
}

void NFAGenerator::processOperator(char op, std::stack<NFA*>& nfaStack) {
    if (op == '*') { // kleene star
        NFA* operand = nfaStack.top();
        nfaStack.pop();

        nfaStack.push(NFA::kleeneStarNFA(operand));
    }
    else if (op == '+') { // Positive closure
        NFA* operand = nfaStack.top();
        nfaStack.pop();

        nfaStack.push(NFA::positiveClosureNFA(operand));
    }
    else if (op == ' ') {
        NFA* rhs = nfaStack.top();
        nfaStack.pop();

        NFA* lhs = nfaStack.top();
        nfaStack.pop();

        nfaStack.push(NFA::concatNAFs(lhs, rhs));
    }
    else if (op == '|') {
        NFA* rhs = nfaStack.top();
        nfaStack.pop();

        NFA* lhs = nfaStack.top();
        nfaStack.pop();

        nfaStack.push(NFA::unionNAFs(lhs, rhs));
    }
    else if (op == '-') { // 0-9 or a-z operator
        NFA* rhs = nfaStack.top();
        nfaStack.pop();

        NFA* lhs = nfaStack.top();
        nfaStack.pop();
        nfaStack.push(NFA::unionRangeNFAs(lhs, rhs));
    }
}

NFA* NFAGenerator::combineNFAs(const std::vector<NFA*>& nfas) {
    NFA* combinedNFA = new NFA();
    NFAState *combinedStart = combinedNFA->getStartState();
    NFAState *combinedEnd = combinedNFA->getEndState();

    for (const NFA* nfa : nfas) {
        combinedStart->addTransition('e', nfa->getStartState());
        nfa->getEndState()->addTransition('e', combinedEnd);
    }

    return combinedNFA;
}

int NFAGenerator::precedence(char op) {
    // Higher value means higher precedence
    if (op == '-') { // assumed higher: range operator
        return 4;
    } else if (op == '*' || op == '+') { // Kleene closure
        return 3;
    } else if (op == ' ') { // Concatenation
        return 2;
    } else if (op == '|') { // Union
        return 1;
    } else {
        return -1;
    }
}