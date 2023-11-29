#include <iostream>
#include <unordered_map>
#include <Lex/NFA.h>
#include <Lex/NFAGenerator.h>


int main() {
    // Example usage
    std::unordered_map<std::string, std::string> regexMap = {
            {"id", "letter (letter|digit)*"},
            {"num", "digit+|digit+ . digits (\\L|E digits)"},
            {"relop", R"(\= \=|!\=|>|> \=|<|< \=)"},
            // Add more regular expressions as needed
    };

    std::unordered_map<std::string, std::string> regexDefMap = {
            {"letter", "a-z|A-Z"},
            {"digit", "0-9"},
            // Add more regular definitions as needed
    };

    std::vector<std::string> keywords = {"boolean", "int", "float", "if", "else", "while"};
    std::vector<std::string> operators = {"+", "-", "*", "/", "=", "<=", "<", ">", ">=", "!=", "=="};
    std::vector<std::string> punctuations = {";", ",", "\\(", "\\)", "{", "}"};

//    NFA* testIdNFA = NFAGenerator::regexToNFA("l (l|d)*");

//    NFA *aNFA = NFA::basicCharToNFA('a');
//    NFA *bNFA = NFA::basicCharToNFA('b');

//    NFA* aOrbNFA = NFA::unionNAFs(aNFA, bNFA);
//    NFA* abNFA = NFA::concatNAFs(aNFA, bNFA);
//    NFA* aStarNFA = NFA::kleeneStarNFA(aNFA);
//    NFA* aPlusNFA = NFA::positiveClosureNFA(aNFA);

    NFAGenerator nfaGenerator;
    NFA* testDigitNFA = nfaGenerator.regexToNFA("a-z|A-Z");

    testDigitNFA->printNFA();
    return 0;
}