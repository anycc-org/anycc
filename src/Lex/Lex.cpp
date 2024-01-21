#include "Lex/Lex.h"
#include "Lex/NFAGenerator.h"
#include "Lex/TransitionDiagramMinimizer.h"
#include "Lex/DeterministicTransitionDiagramCreator.h"


Lex::~Lex() {
    delete rules;
    delete inputReader;
    delete analyzer;
}

Lex::Lex(std::string &rules_file_name, std::string &program_file_name) {
    this->rules_file_name = rules_file_name;
    this->program_file_name = program_file_name;
}

void Lex::buildLex() {
    read_rules();
    NFA *nfa = buildNFA();
    auto *transition_diagram = buildDFA(nfa);

    analyzer = new Analyzer(program_file_name, transition_diagram->getStartState(), transition_diagram);
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

TransitionDiagram *Lex::buildDFA(NFA *nfa) {
    auto *transition_diagram = new TransitionDiagram(nfa->getStartState(), nfa->getEndStates(), rules->getTokens(),
                                                     rules->getTokensPriority());

    printTransitionDiagramStatistics(transition_diagram, "Converted to NFA");

    createDiagramWithDifferentTypes(transition_diagram, "nfa");

    transition_diagram = DeterministicTransitionDiagramCreator::subsetConstruction(transition_diagram);

    printTransitionDiagramStatistics(transition_diagram, "Converted to DFA");

    createDiagramWithDifferentTypes(transition_diagram, "dfa");

    transition_diagram = TransitionDiagramMinimizer::minimize(transition_diagram);

    printTransitionDiagramStatistics(transition_diagram, "Minimized DFA");

    createDiagramWithDifferentTypes(transition_diagram, "minimized_dfa");

    return transition_diagram;
}

void Lex::createDiagramWithDifferentTypes(TransitionDiagram *transition_diagram, const std::string &title) {
    transition_diagram->toDotFile(title + ".dot");
    transition_diagram->toCSVFile(title + ".csv");
    transition_diagram->toMDFile(title + ".md");
}

void Lex::printTransitionDiagramStatistics(TransitionDiagram *transition_diagram, const std::string &title) {
    std::cout << title << "\n\n";
    std::cout << "Number of States = " << transition_diagram->getStates().size() << "\n";
    std::cout << "Number of End States = " << transition_diagram->getEndStates().size() << "\n";
    std::cout << "Number of Dead States = " << transition_diagram->getDeadStates().size() << "\n\n";
}

void Lex::getAllTokensAndCreateOutputFile() {
    std::ofstream tokens_file("../output/tokens.txt");
    Token *token;
    while ((token = analyzer->getNextTokenInQueue()) != nullptr) {
        tokens_file << "{" << *(token->getKey()) << " -> " << *(token->getValue()) << "}" << '\n';
        std::cout << "{" << *(token->getKey()) << " -> " << *(token->getValue()) << "}" << '\n';
    }
    tokens_file.close();
    std::cout << '\n';
}

Token *Lex::getNextToken() {
    return analyzer->getNextToken();
}
