#include <Lex/Lex.h>
#include <Lex/NFAGenerator.h>
#include <Lex/TransitionDiagram.h>
#include <Lex/TransitionDiagramMinimizer.h>
#include <Lex/DeterministicTransitionDiagramCreator.h>
#include "Analyzer.h"


Lex::~Lex() {
    delete rules;
    delete inputReader;
    delete rules_file_name;
    delete program_file_name;
}

Lex::Lex(std::string *rules_file_name, std::string *program_file_name) {
    this->rules_file_name = rules_file_name;
    this->program_file_name = program_file_name;
}

void Lex::buildLex() {
    read_rules();
    NFA *nfa = buildNFA();

    auto *table = new TransitionDiagram(nfa->getStartState(), nfa->getEndStates());
    // table->print();
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";
    table->toDotFile("nfa.dot");
    DeterministicTransitionDiagramCreator dfaCreator;
    table = dfaCreator.subsetConstruction(table);
    std::cout << "converted to dfa\n";
    table->toDotFile("dfa.dot");
    // table->print();
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";
    TransitionDiagramMinimizer minimizer;
    table = minimizer.minimize(table);
    table->toDotFile("min_dfa.dot");
    std::cout << "minimized dfa\n";
    // table->print();
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";

    Analyzer analyzer(*program_file_name, table->getStartState(), table);
    analyzer.analyzeProgram();

    Token *token;
    while ((token = analyzer.getNextToken()) == nullptr) {
        std::cout << token->getKey() << " " << token->getValue() << '\n';
    }
}

void Lex::read_rules() {
    rules = new Rules();
    inputReader = new InputReader(this->rules_file_name, rules);

    rules->printRules();
}

NFA *Lex::buildNFA() {
    auto *NfaGenerator = new NFAGenerator();
    return NfaGenerator->buildNFA(rules->getRegularExpressions(), rules->getRegularDefinitions(), rules->getKeywords(),
                                  rules->getPunctuations());
}
