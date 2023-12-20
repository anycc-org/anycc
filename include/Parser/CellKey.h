#ifndef ANYCC_CELLKEY_H
#define ANYCC_CELLKEY_H

#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>

class CellKey {
public:
    CellKey(const std::string &nonTerminal, const std::string &terminal);

    ~CellKey();

    const std::string &getNonTerminal() const;

    const std::string &getTerminal() const;

    bool operator==(const CellKey &cell_key) const {
        return non_terminal + terminal == cell_key.non_terminal + cell_key.terminal;
    }

private:
    std::string non_terminal;
    std::string terminal;
};

class CellKeyHashFn {
public:
    size_t operator()(const CellKey &cell_key) const {
        return std::hash<std::string>{}(cell_key.getNonTerminal() + cell_key.getTerminal());
    }
};


#endif
