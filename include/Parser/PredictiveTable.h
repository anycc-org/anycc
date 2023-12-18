#ifndef ANYCC_PREDICTIVETABLE_H
#define ANYCC_PREDICTIVETABLE_H

#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include "Production.h"
#include "CellKey.h"
#include "CellValue.h"
#include "FirstAndFollowGenerator.h"

class PredictiveTable {
public:
    PredictiveTable(
            const std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> &computed_first_sets,
            const std::unordered_map<std::string, std::set<std::string>> &computed_follow_sets,
            const std::set<std::string> &non_terminals);

    ~PredictiveTable();

    void buildPredictiveTable();

    CellValue *lookUp(const std::string &non_terminal, const std::string &terminal);

    PredictiveTableEnum getCellType(const std::string &non_terminal, const std::string &terminal);

    bool hasProduction(const std::string &non_terminal, const std::string &terminal);

    bool isCellEmpty(const std::string &non_terminal, const std::string &terminal);

    bool isSynchronizing(const std::string &non_terminal, const std::string &terminal);

    void printPredictiveTable();


private:
    std::unordered_map<CellKey, CellValue *, CellKeyHashFn> predictive_table;
    std::set<std::string> non_terminals;
    std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> computed_first_sets;
    std::unordered_map<std::string, std::set<std::string>> computed_follow_sets;

    void insertFirstSets();

    void insertEpsilonAtFollowSet(const std::string &non_terminal, Production &production,
                                  std::set<std::string> &follow_set);

    void addSynchAtFollowSetElements(const std::string &non_terminal, std::set<std::string> &follow_set);

    void insertProduction(const std::string &non_terminal, const std::string &terminal, Production &production,
                          PredictiveTableEnum predictive_table_enum);

    bool isEpsilon(const std::pair<std::string, Production> &first) const;

    bool containsKey(const std::string &non_terminal, const std::basic_string<char> &follow);

    void printConflict(const std::string &non_terminal, const std::string &terminal, Production &production);
};


#endif
