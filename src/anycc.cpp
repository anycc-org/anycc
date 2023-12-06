#include "Lex/TransitionDiagramMinimizer.h"
#include "Lex/DeterministicTransitionDiagramCreator.h"
#include "Lex/NFAState.h"
#include <iostream>
#include <unordered_map>
#include <Lex/NFA.h>
#include <Lex/NFAGenerator.h>
#include <map>
#include <Lex/TransitionDiagram.h>

int main() {
    // Example usage
    std::unordered_map<std::string, std::string> regexMap = {
//            {"id", "letter (letter|digit)*"},
//            {"num", "digit+|digit+ . digits (\\L|E digits)"},
//            {"relop", R"(\= \=|! \=|>|> \=|<|< \=)"},
//            {"assign", "="},
//            {"addop", "\\+|\\-"},
//            {"mulop", "\\*|/"},
            // Add more regular expressions as needed
    };

    std::vector<std::pair<std::string, std::string>> regexDefMap = {
//            {"letter", "a-z|A-Z"},
//            {"digit", "0-9"},
//            {"digits", "digit+"},
            // Add more regular definitions as needed
    };

    std::vector<std::string> keywords = {"boolean", "int", "float", "if", "else", "while"};
    std::vector<std::string> operators = {"+", "-", "*", "/", "=", "<=", "<", ">", ">=", "!=", "=="};
    std::vector<std::string> punctuations = {";", ",", "\\(", "\\)", "{", "}"};

//    NFA* idNFA = NFAGenerator::regexToNFA("l (l|d)*");

//    NFA *aNFA = NFA::basicCharToNFA('a');
//    NFA *bNFA = NFA::basicCharToNFA('b');

//    NFA* aOrbNFA = NFA::unionNAFs(aNFA, bNFA);
//    NFA* abNFA = NFA::concatNAFs(aNFA, bNFA);
//    NFA* aStarNFA = NFA::kleeneStarNFA(aNFA);
//    NFA* aPlusNFA = NFA::positiveClosureNFA(aNFA);

    NFAGenerator nfaGenerator;

//    NFA* sentenceNFA = nfaGenerator.regexToNFA("ab cd");

//    NFA* addopNFA = nfaGenerator.regexToNFA("\\+|\\-");
//    NFA* mulopNFA = nfaGenerator.regexToNFA("\\*|/");
//    NFA* relopNFA = nfaGenerator.regexToNFA(R"(\= \=|! \=|>|> \=|<|< \=)");

    NFA* digitNFA = nfaGenerator.regexToNFA("ab");
    digitNFA->printNFA();
    std::cout << digitNFA->getEndState()->getStateId() << "\n";
    for(auto s : digitNFA->getEndStates()) {
        std::cout << s->getStateId() << " ";
    }
    std::cout << "\n";
    TransitionDiagram* table = new TransitionDiagram(digitNFA->getStartState(), std::vector<const NFAState*>{digitNFA->getEndState()});

    table = table->removeEpsilonTransitions();

    DeterministicTransitionDiagramCreator dfaCreator;
    table = dfaCreator.subsetConstruction(table);
    TransitionDiagramMinimizer minimizer;
    table = minimizer.minimize(table);
    table->print();

//    NFA* wordNFA = NFA::wordToNFA("{");
//    wordNFA->printNFA();

//     NFA* testNFA = nfaGenerator.buildNFA(regexMap, regexDefMap, keywords, operators, punctuations);
// //
//     testNFA->printNFA();

    // Create an original NFA
//    NFA* originalNFA = NFA::basicCharToNFA('a');

    // Use the copy constructor to create a deep copy
//    NFA* copiedNFA = new NFA(*originalNFA);

    // Print the copied NFA
//    std::cout << "Copied NFA:" << std::endl;
//    copiedNFA->printNFA();
//    std::cout << std::endl;

    // Cleanup memory
//    delete originalNFA;
//    delete copiedNFA;



    return 0;
}