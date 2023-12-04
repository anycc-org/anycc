#include <Lex/Lex.h>


Lex::~Lex() {
    delete rules;
    delete inputReader;
}

Lex::Lex(std::string *rules_file_name, std::string *program_file_name) {
    this->rules_file_name = rules_file_name;
    this->program_file_name = program_file_name;
}

void Lex::read_rules() {
    rules = new Rules();
    inputReader = new InputReader(this->rules_file_name, rules);

    rules->printRules();
}
