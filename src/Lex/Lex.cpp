#include <Lex/Lex.h>
#include <Lex/NFAGenerator.h>
#include <Lex/TransitionDiagram.h>
#include <Lex/TransitionDiagramMinimizer.h>
#include <Lex/DeterministicTransitionDiagramCreator.h>


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
    // nfa->printNFA();
    for(auto kv : nfa->getEndState()->getTransitions()) {
        std::cout << kv.first << " ";
    }
    std::cout << "============================================\n";
    std::cout << nfa->getStartState()->getStateId() << "\n";
    std::cout << "============================================\n";
    TransitionDiagram* table = new TransitionDiagram(nfa->getStartState(), nfa->getEndStates());
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";
    std::cout << "\n";
    table = table->removeEpsilonTransitions();
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";
    std::cout << "removed eps\n";
    table->print();
    DeterministicTransitionDiagramCreator dfaCreator;
    table = dfaCreator.subsetConstruction(table);
    std::cout << "converted to dfa\n";
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";
    for(auto c : table->getInputs()) {
        std::cout << c << " ";
    }
    std::cout << "\n";
    TransitionDiagramMinimizer minimizer;
    table = minimizer.minimize(table);
    std::cout << "minimized dfa\n";
    std::cout << table->getStates().size() << "\n";
    std::cout << table->getEndStates().size() << "\n";
    std::cout << table->getDeadStates().size() << "\n";
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
