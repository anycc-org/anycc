#include "Lex/TransitionDiagramMinimizer.h"
#include "Lex/DeterministicTransitionDiagramCreator.h"
#include "Lex/NFAState.h"
#include <iostream>
#include <unordered_map>
#include <Lex/NFA.h>
#include <Lex/NFAGenerator.h>
#include <map>
#include <Lex/TransitionDiagram.h>
#include <Lex/TransitionDiagram.h>
#include <Lex/Lex.h>

int main() {

    auto *rules_file_name = new std::string("rules.txt");
    auto *program_file_name = new std::string("/program.txt");
    Lex *lex = new Lex(rules_file_name, program_file_name);

    lex->buildLex();

//    NFA* idNFA = NFAGenerator::regexToNFA("l (l|d)*");

//    NFA *aNFA = NFA::basicCharToNFA('a');
//    NFA *bNFA = NFA::basicCharToNFA('b');

//    NFA* aOrbNFA = NFA::unionNAFs(aNFA, bNFA);
//    NFA* abNFA = NFA::concatNAFs(aNFA, bNFA);
//    NFA* aStarNFA = NFA::kleeneStarNFA(aNFA);
//    NFA* aPlusNFA = NFA::positiveClosureNFA(aNFA);


//    NFA* sentenceNFA = nfaGenerator.regexToNFA("ab cd");

//    NFA* addopNFA = nfaGene   rator.regexToNFA("\\+|\\-");
//    NFA* mulopNFA = nfaGenerator.regexToNFA("\\*|/");
//    NFA* relopNFA = nfaGenerator.regexToNFA(R"(\= \=|! \=|>|> \=|<|< \=)");

   
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



}