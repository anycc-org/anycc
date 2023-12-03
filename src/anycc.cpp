#include <iostream>
#include <Lex/NFA.h>
#include <Lex/NFAGenerator.h>
#include <map>

NFAGenerator nfaGenerator;

void testIdNFA() {
    std::vector<std::pair<std::string, std::string>> regexDefMap = {
            {"letter", "a-z|A-Z"},
            {"digit", "0-9"},
    };
    std::vector<std::pair<std::string, std::string>> regexMap = {
            {"id", "letter (letter|digit)*"},
    };
    NFA* idNFA = nfaGenerator.buildNFA(regexMap, regexDefMap, {}, {});
    idNFA->printNFA();
}

void testNFAConcatTwoSymbols() {
    NFA* aNFA = NFA::basicCharToNFA('a');
    NFA* bNFA = NFA::basicCharToNFA('b');
    NFA* abNFA = NFA::concatNAFs(aNFA, bNFA);
    abNFA->printNFA();
}

void testNFAUnionTwoSymbols() {
    NFA* aNFA = NFA::basicCharToNFA('a');
    NFA* bNFA = NFA::basicCharToNFA('b');
    NFA* aOrbNFA = NFA::unionNAFs(aNFA, bNFA);
    aOrbNFA->printNFA();
}

void testNFAUnionWords() {
    NFA* wordNFA = NFA::wordToNFA("abc|def");
    wordNFA->printNFA();
}

void testNFAConcatWord() {
    NFA* wordNFA = NFA::wordToNFA("abc");
    wordNFA->printNFA();
}

void testNFAConcatWords() {
    NFA* wordNFA = NFA::wordToNFA("abc def");
    wordNFA->printNFA();
}

void testNFAKleeneStar() {
    NFA* aNFA = NFA::basicCharToNFA('a');
    NFA* aStarNFA = NFA::kleeneStarNFA(aNFA);
    aStarNFA->printNFA();
}

void testNFAKleeneStarWord() {
    NFA* wordNFA = NFA::wordToNFA("(abc)*");
    wordNFA->printNFA();
}

void testNFArelop() {
    NFA* relopNFA = nfaGenerator.regexToNFA(R"(\= \=|! \=|>|> \=|<|< \=)");
    relopNFA->printNFA();
}

void testNFAaddop() {
    NFA* addopNFA = nfaGenerator.regexToNFA("\\+|\\-");
    addopNFA->printNFA();
}

void testNFAmulop() {
    NFA* mulopNFA = nfaGenerator.regexToNFA("\\*|/");
    mulopNFA->printNFA();
}

int main() {
    // Example usage
    std::vector<std::pair<std::string, std::string>> regexMap = {
            {"id", "letter (letter|digit)*"},
            {"num", "digit+|digit+ . digits (\\L|E digits)"},
            {"relop", R"(\= \=|! \=|>|> \=|<|< \=)"},
            {"assign", "="},
            {"addop", "\\+|\\-"},
            {"mulop", "\\*|/"},
    };

    std::vector<std::pair<std::string, std::string>> regexDefMap = {
            {"letter", "a-z|A-Z"},
            {"digit", "0-9"},
            {"digits", "digit+"},
    };

    std::vector<std::string> keywords = {"boolean", "int", "float", "if", "else", "while"};
    std::vector<std::string> punctuations = {";", ",", "\\(", "\\)", "{", "}"};

    testIdNFA();
    return 0;
}