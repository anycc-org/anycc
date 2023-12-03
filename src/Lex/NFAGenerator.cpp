#include <Lex/NFAGenerator.h>
#include <stack>
#include <iostream>
#include <map>

NFAGenerator::NFAGenerator() = default;

NFAGenerator::~NFAGenerator() = default;

NFA *NFAGenerator::buildNFA(const std::unordered_map<std::string, std::string> &regexMap,
                            const std::vector<std::pair<std::string, std::string>> &regexDefMap,
                            const std::vector<std::string> &keywords,
                            const std::vector<std::string> &operators,
                            const std::vector<std::string> &punctuations) {
    std::vector<NFA*> nfas;
    for (auto& regexDef : regexDefMap) {
        NFA* nfa = regexToNFA(regexDef.second);
        nfas.push_back(nfa);
        regexToNFAMap[regexDef.first] = nfa;
    }

    for (auto& regex : regexMap) {
        nfas.push_back(regexToNFA(regex.second));
    }

    for (auto& keyword : keywords) {
        nfas.push_back(NFA::wordToNFA(keyword));
    }

    for (auto& op : operators) {
        nfas.push_back(NFA::wordToNFA(op));
    }

    for (auto& punc : punctuations) {
        nfas.push_back(NFA::wordToNFA(punc));
    }

    return combineNFAs(nfas);
}

/**
 * @return NFA after regex evaluation (postfix)
 */
NFA* NFAGenerator::regexToNFA(const std::string& regex) {
    size_t n = regex.size();
    std::stack<NFA*> nfaStack;
    std::stack<char> operatorStack; // word + (space or '\' or operator)
    int i;

    for (i = 0; i < n; i++) { // {"l (l|d)*", "\+|-", "d+|d+ . s (\L|E s)"
        char c = regex[i];
        if (c == '\\') { // escape-backslash for reserved symbols
            if (i + 1 < n && regex[i + 1] == 'L') { // epsilon
                nfaStack.push(NFA::basicCharToNFA('e'));
            }
            else { // eg. \+ \* \= \( \)
                nfaStack.push(NFA::basicCharToNFA(regex[i + 1]));
            }
            i++;
        }
        else if (isOperator(c)) { // Operator {*, +, ' ', |, -}
            while (!operatorStack.empty() && precedence(operatorStack.top()) >= precedence(c)) {
                char op = operatorStack.top();
                operatorStack.pop();

                processOperator(op, nfaStack);
            }
            operatorStack.push(c);
        }
        else if (c == '(') {
            operatorStack.push('(');
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
        else { // symbol encountered
            // try to form a complete word
            std::string word = std::string(1, c);
            while (i + 1 < n && !isOperator(regex[i + 1]) && regex[i + 1] != '(' && regex[i + 1] != ')') {
                word += regex[++i];
            }
            // check if that word in the regular definition map
            if (regexToNFAMap.find(word) != regexToNFAMap.end()) {
                // take a deep copy from this nfa
                nfaStack.push(new NFA(*regexToNFAMap[word]));
            }
            else {
                if (word.size() == 1) {
                    nfaStack.push(NFA::basicCharToNFA(c));
                }
                else {
                    // form an NFA by concatenating the characters of that word
                    nfaStack.push(NFA::wordToNFA(word));
                }
            }
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

int NFAGenerator::precedence(char op) const {
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

bool NFAGenerator::isOperator(char op) const {
    return op == ' '|| op == '|' || op == '*' || op == '+' || op == '-';
}
