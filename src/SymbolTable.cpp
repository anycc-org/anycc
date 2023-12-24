#include "SymbolTable.h"
#include <iostream>

void
SymbolTable::insertEntry(std::string &word, std::string &token_name, int token_id, int line_number, int column_number) {
    if (table.find(word) != table.end())
        return;

    auto *entry = new SymbolTableEntry();
    entry->token_name = token_name;
    entry->token_id = token_id;
    entry->line_number = line_number;
    entry->column_number = column_number;

    table[word] = entry;
}

SymbolTableEntry *SymbolTable::getEntry(std::string &lexeme) {
    return table[lexeme];
}

void SymbolTable::printTable() {
    std::cout << "\n" << "Symbol Table" << "\n\n";
    for (const auto &entry: table) {
        std::cout << entry.first << " : " << entry.second->token_name << " : "
                  << entry.second->line_number << " : " << entry.second->column_number << std::endl;
    }
    std::cout << "\n\n";
}
