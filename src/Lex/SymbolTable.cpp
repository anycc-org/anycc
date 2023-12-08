#include <Lex/SymbolTable.h>
#include <iostream>

void SymbolTable::insertEntry(std::string &word, std::string &token_name, int token_id, int line_number) {
    if (table.find(word) != table.end())
        return;

    auto *entry = new SymbolTableEntry();
    entry->token_name = token_name;
    entry->token_id = token_id;
    entry->line_number = line_number;

    table[word] = entry;
}

SymbolTableEntry *SymbolTable::getEntry(std::string &lexeme) {
    return table[lexeme];
}

void SymbolTable::printTable() {
    std::cout << "\n\n" << "Symbol Table:" << "\n";
    for (const auto &entry: table) {
        std::cout << entry.first << " : " << entry.second->token_name << " : "
                  << entry.second->line_number << std::endl;
    }
    std::cout << "\n\n";
}
