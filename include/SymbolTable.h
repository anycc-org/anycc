#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

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
    /**
     * @brief Get the instance of the singleton class
     * @return The instance of the singleton class
     */
    static SymbolTable &getInstance() {
        static SymbolTable instance;
        return instance;
    }

    /**
     * @brief Insert a new entry to the symbol table
     * @param word The word to be inserted
     * @param token_name The token name of the word
     * @param token_id The token id of the word
     * @param line_number The line number of the word
     * @param column_number The column number of the word
     */
    void insertEntry(std::string &word, std::string &token_name, int token_id, int line_number, int column_number);

    /**
     * @brief Get the entry of the given lexeme
     * @param lexeme The lexeme to be searched for
     * @return The entry of the given lexeme
     */
    SymbolTableEntry *getEntry(std::string &lexeme);

    const std::unordered_map<std::string, SymbolTableEntry *> &getTable() const;

    /**
     * @brief Print the symbol table
     */
    void generateMarkdownTable(const std::string &filename);

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

#endif // SYMBOLTABLE_H