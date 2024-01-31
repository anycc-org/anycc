#include "SymbolTable.h"
#include <iostream>
#include <fstream>

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

void SymbolTable::generateMarkdownTable(const std::string &filename) {
    std::ofstream tableFile(filename);
    tableFile << "### Symbol Table\n\n";
    tableFile << "| Key  | Value | Line Number | Column Number |\n";
    tableFile << "|--------|---------------|------------|---------------|\n";

    for (const auto &entry: table) {
        tableFile << "| " << entry.first << " | " << entry.second->token_name << " | "
                  << entry.second->line_number << " | " << entry.second->column_number << " |\n";
    }

    tableFile.close();
}

const std::unordered_map<std::string, SymbolTableEntry *> &SymbolTable::getTable() const {
    return table;
}
