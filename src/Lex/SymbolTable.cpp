#include <Lex/SymbolTable.h>

void SymbolTable::insertEntry(std::string &word, std::string &token_name, int token_id, int line_number) {
    auto *entry = new SymbolTableEntry();
    entry->token_name = token_name;
    entry->token_id = token_id;
    entry->line_number = line_number;

    table[word] = entry;
}

SymbolTableEntry *SymbolTable::getEntry(std::string &lexeme) {
    return table[lexeme];
}
