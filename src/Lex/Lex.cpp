#include <Lex/Lex.h>
#include <Lex/NFAGenerator.h>
#include <Lex/TransitionDiagram.h>
#include <Lex/TransitionDiagramMinimizer.h>
#include <Lex/DeterministicTransitionDiagramCreator.h>
#include "Analyzer.h"


Lex::~Lex() {
    delete rules;
    delete inputReader;
}

Lex::Lex(std::string *rules_file_name, std::string *program_file_name) {
    this->rules_file_name = rules_file_name;
    this->program_file_name = program_file_name;
}

void Lex::buildLex() {
    read_rules();
    NFA *nfa = buildNFA();
    TransitionDiagram *table = new TransitionDiagram(nfa->getStartState(), nfa->getEndStates(), rules->getTokens(),
                                                     rules->getTokensPriority());
    // table->print();
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";
    table->toDotFile("nfa.dot");
    table->toCSVFile("nfa.csv");
    table->toMDFile("nfa.md");
    DeterministicTransitionDiagramCreator dfaCreator;
    table = dfaCreator.subsetConstruction(table);
    std::cout << "converted to dfa\n";
    table->toDotFile("dfa.dot");
    table->toCSVFile("dfa.csv");
    table->toMDFile("dfa.md");
    // table->print();
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";
    TransitionDiagramMinimizer minimizer;
    table = minimizer.minimize(table);
    table->toDotFile("min_dfa.dot");
    table->toCSVFile("min_dfa.csv");
    table->toMDFile("min_dfa.md");
    std::cout << "minimized dfa\n";
    // table->print();
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";
    for (auto kv: table->getEndStatesTokensMap()) {
        std::cout << kv.first->getStateId() << ": " << kv.second << "\n";
    }
    std::cout << table->getEndStatesTokensMap().size();
    std::cout << "\n";
    for (auto s: rules->getTokens()) {
        std::cout << s << "\n";
    }
    std::cout << rules->getTokens().size();
    std::cout << "\n";

    Analyzer analyzer(*program_file_name, table->getStartState(), table);
    analyzer.analyzeProgram();

    std::ofstream tokens_file("tokens.txt");
    Token *token;
    while ((token = analyzer.getNextToken()) != nullptr) {
        tokens_file << "{" << *(token->getKey()) << " -> " << *(token->getValue()) << "}" << '\n';
        std::cout << "{" << *(token->getKey()) << " -> " << *(token->getValue()) << "}" << '\n';
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
