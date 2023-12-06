#pragma once

#include <string>
#include <unordered_map>

struct SymbolTableEntry {
    std::string token_name;
    std::string token_type;
    int token_id;
    int line_number;
    int column_number;
};

class SymbolTable {
public:
    static SymbolTable &getInstance() {
        static SymbolTable instance;
        return instance;
    }

    void insertEntry(std::string &word, std::string &token_name, int token_id, int line_number);

    SymbolTableEntry *getEntry(std::string &lexeme);

    SymbolTable(const SymbolTable &) = delete;

    void operator=(const SymbolTable &) = delete;

private:
    std::unordered_map<std::string, SymbolTableEntry *> table;

    SymbolTable() {
        table = std::unordered_map<std::string, SymbolTableEntry *>();
    }

    ~SymbolTable() {
        for (auto &entry: table)
            delete entry.second;
    };
};