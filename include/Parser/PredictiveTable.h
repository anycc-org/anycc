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

    /**
     * @brief Build the predictive table
     */
    void buildPredictiveTable();

    /**
     * @brief Gets pointer to cell value
     * @warning the responsibility of deleting pointer is the caller's
     * @param non_terminal The LHS to get the production from
     * @param terminal The string which moves non_terminal to the production
     * @return Pointer to cell value
     */
    const CellValue *lookUp(const std::string &non_terminal, const std::string &terminal);

    /**
     * @brief Gets the type of the cell
     * @param non_terminal The LHS to get the production from
     * @param terminal The string which moves non_terminal to the production
     * @return The type of the cell
     */
    ParsingTableEntryType getCellType(const std::string &non_terminal, const std::string &terminal);

    /**
     * @brief Checks if the cell is empty
     * @param non_terminal The LHS to get the production from
     * @param terminal The string which moves non_terminal to the production
     * @return True if the cell isn't empty, false otherwise
     */
    bool hasProduction(const std::string &non_terminal, const std::string &terminal);

    /**
     * @brief Checks if the cell is empty
     * @param non_terminal The LHS to get the production from
     * @param terminal The string which moves non_terminal to the production
     * @return True if the cell is empty, false otherwise
     */
    bool isCellEmpty(const std::string &non_terminal, const std::string &terminal);

    /**
     * @brief Checks if the cell is synchronizing
     * @param non_terminal The LHS to get the production from
     * @param terminal The string which moves non_terminal to the production
     * @return True if the cell is synchronizing, false otherwise
     */
    bool isSynchronizing(const std::string &non_terminal, const std::string &terminal);

    /**
     * @brief Prints the predictive table
     */
    void printPredictiveTable();


private:
    std::unordered_map<CellKey, CellValue *, CellKeyHashFn> predictive_table;
    std::set<std::string> non_terminals;
    std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> computed_first_sets;
    std::unordered_map<std::string, std::set<std::string>> computed_follow_sets;

    /**
     * @brief For each terminal `a` in `FIRST(α)` do: Add `A -> α` to `M[A, a]`.
     */
    void insertFirstSets();

    /**
     * @brief - If `Є` is in `FIRST(α)` then: For each terminal `b` in `FOLLOW(A)` do: Add `A -> α` to `M[A, b]`.
              - If `Є` is in `FIRST(α)` and `$` is in `FOLLOW(A)` then: Add `A -> α` to `M[A, $]`.
     */
    void insertEpsilonAtFollowSet(const std::string &non_terminal, const Production &production,
                                  const std::set<std::string> &follow_set);

    /**
     * @brief For each terminal `a` in `FOLLOW(A)` do: Add `synch` to `M[A, a]`.
     */
    void addSynchAtFollowSetElements(const std::string &non_terminal, const std::set<std::string> &follow_set);

    /**
     * @brief Insert the given production at the given cell
     * @param non_terminal The LHS to get the production from
     * @param terminal The string which moves non_terminal to the production
     * @param production The production to be inserted
     * @param predictive_table_enum The type of the cell
     */
    void insertProduction(const std::string &non_terminal, const std::string &terminal, const Production &production,
                          ParsingTableEntryType predictive_table_enum);

    /**
     * @brief Checks if the given production is epsilon
     * @param first element from the first set
     * @return True if the given first is epsilon, false otherwise
     */
    static bool isEpsilon(const std::pair<std::string, const Production> &first);

    /**
     * @brief Checks if the predictive table contains the given key
     * @param non_terminal The LHS to get the production from
     * @param terminal The string which moves non_terminal to the production
     * @return True if the key exists, false otherwise
     */
    bool containsKey(const std::string &non_terminal, const std::basic_string<char> &follow);

    /**
     * @brief Prints the conflict
     * @param non_terminal The LHS to get the production from
     * @param terminal The string which moves non_terminal to the production
     * @param production The production made the conflict
     */
    void printConflict(const std::string &non_terminal, const std::string &terminal, const Production &production);
};


#endif
