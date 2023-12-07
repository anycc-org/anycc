#include <Lex/Lex.h>
#include "NFAGenerator.h"


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
    nfa->printNFA();
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
