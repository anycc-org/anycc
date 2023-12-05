#include <Lex/SymbolTable.h>

void SymbolTable::insertEntry(std::string &word, std::string &token_name, int token_id, int line_declaration_number) {
    auto *entry = new SymbolTableEntry();
    entry->token_name = token_name;
    entry->token_id = token_id;
    entry->line_declaration_number = line_declaration_number;

    table[word] = entry;
}

SymbolTableEntry *SymbolTable::getEntry(std::string &lexeme) {
    return table[lexeme];
}
