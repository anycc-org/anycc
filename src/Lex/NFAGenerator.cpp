#include "Lex/NFAGenerator.h"
#include <stack>
#include <iostream>
#include "Lex/Epsilon.h"

NFAGenerator::NFAGenerator() = default;

NFAGenerator::~NFAGenerator() {
    regexToNFAMap.clear();
}

/**
 * @brief t converts each regex definition, regex expression, keyword, and punctuation
 * into corresponding NFAs and then combines them into a single NFA.
 * @return NFA after combining all NFAs
 */
NFA *NFAGenerator::buildNFA(const std::vector<Token*>& regexMap,
                            const std::vector<Token*>& regexDefMap,
                            const std::vector<std::string>& keywords,
                            const std::vector<std::string>& punctuations) {
    // Create a vector to store individual NFAs
    std::vector<NFA*> nfas;

    // Convert regular definitions to NFAs and add them to the vector
    for (auto& regexDef : regexDefMap) {
        NFA* nfa = regexToNFA(*regexDef->getValue());
        nfa->setTokenName(*regexDef->getKey());
        regexToNFAMap[*regexDef->getKey()] = nfa;
    }

    // Convert regular expressions to NFAs and add them to the vector
    for (auto& regex : regexMap) {
        NFA* nfa = regexToNFA(*regex->getValue());
        nfa->setTokenName(*regex->getKey());
        nfas.push_back(nfa);
    }

    // Convert keywords and punctuations to NFAs and add them to the vector
    for (auto& keyword : keywords) {
        NFA* nfa = NFA::wordToNFA(keyword);
        nfa->setTokenName(keyword);
        nfas.push_back(nfa);
    }

    for (auto& punc : punctuations) {
        NFA* nfa = NFA::wordToNFA(punc);
        nfa->setTokenName(punc);
        nfas.push_back(nfa);
    }

    // Combine all NFAs in the vector into a single NFA and return it
    return combineNFAs(nfas);
}

/**
 * @brief Converts a regex expression to an NFA using a stack-based approach.
 * @return NFA after regex evaluation (postfix)
 */
NFA* NFAGenerator::regexToNFA(const std::string& regex) {
    size_t n = regex.size();
    std::stack<NFA*> nfaStack;
    std::stack<char> operatorStack;
    int i;

    for (i = 0; i < n; i++) {
        char c = regex[i];

        if (isOperator(c)) { // Operator {*, +, ' ', |, -}
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
            std::string word = "";
            while (i < n && !isOperator(regex[i]) && regex[i] != '(' && regex[i] != ')') {
                if (regex[i] == '\\') {
                    word += regex[i];
                    if (i + 1 < n) {
                        word += regex[++i];
                    }
                }
                else {
                    word += regex[i];
                }
                i++;
            }
            if (i < n && (isOperator(regex[i]) || regex[i] == '(' || regex[i] == ')')) {
                i--;
            }
            // check if that word in the regular definition map
            if (regexToNFAMap.find(word) != regexToNFAMap.end()) {
                // take a deep copy from this nfa
                nfaStack.push(new NFA(*regexToNFAMap[word]));
            }
            // handle cases like bbb*|aaa*
            else if (word.size() > 1 && i + 1 < n && (regex[i + 1] == '*' || regex[i + 1] == '+')) {
                NFA* subWordNFA = NFA::wordToNFA(word.substr(0, word.size() - 1));
                NFA* kleenNFA;
                if (regex[i + 1] == '*') {
                    kleenNFA = NFA::kleeneStarNFA(NFA::basicCharToNFA(word[word.size() - 1]));
                }
                else {
                    kleenNFA = NFA::positiveClosureNFA(NFA::basicCharToNFA(word[word.size() - 1]));
                }
                i++;
                nfaStack.push(NFA::concatNAFs(subWordNFA, kleenNFA));
            }
            else {
                // form an NFA by concatenating the characters of that word
                nfaStack.push(NFA::wordToNFA(word));
            }
        }
    }

    while (!operatorStack.empty()) {
        char op = operatorStack.top();
        operatorStack.pop();

        processOperator(op, nfaStack);
    }

    if (nfaStack.size() != 1) {
        std::cerr << "Invalid regex: " << regex << std::endl;
        exit(1);
    }

    // Top of the stack contains the final NFA
    NFA* resultNFA = nfaStack.top();
    nfaStack.pop();
    resultNFA->addEndState(resultNFA->getEndState());
    return resultNFA;
}

/**
 * @brief Processes operators and performs corresponding operations on the NFA stack.
 */
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
    else {
        std::cerr << "Invalid operator: " << op << std::endl;
    }
}

/**
 * @brief Combines multiple NFAs into a single NFA by creating epsilon transitions
 * from a combined start state to the start states of individual NFAs and
 * adding their end states to the combined end states.
 * */
NFA* NFAGenerator::combineNFAs(std::vector<NFA*>& nfas) {
    NFA* combinedNFA = new NFA();
    NFAState *combinedStart = combinedNFA->getStartState();

    for (const NFA* nfa : nfas) {
        combinedStart->addTransition(EPSILON, nfa->getStartState());
        combinedNFA->addEndState(nfa->getEndState());
    }

    nfas.clear();
    return combinedNFA;
}

/**
 * Higher value means higher precedence
 * @return precedence of the operator
 */
int NFAGenerator::precedence(char op) const {
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

/**
 * @return true if it's a supported operator
 */
bool NFAGenerator::isOperator(char op) const {
    return op == ' '|| op == '|' || op == '*' || op == '+' || op == '-';
}