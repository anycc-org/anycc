#include "Parser/PredictiveTable.h"
#include "constants.h"

PredictiveTable::PredictiveTable(
        const std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> &computed_first_sets,
        const std::unordered_map<std::string, std::set<std::string>> &computed_follow_sets,
        const std::set<std::string> &non_terminals) {
    this->computed_first_sets = computed_first_sets;
    this->computed_follow_sets = computed_follow_sets;
    this->non_terminals = non_terminals;
}

PredictiveTable::~PredictiveTable() {
    for (const auto &element: predictive_table)
        delete element.second;
}

void PredictiveTable::buildPredictiveTable() {
    insertFirstSets();
    for (const auto &non_terminal: non_terminals) {
        addSynchAtFollowSetElements(non_terminal, computed_follow_sets[non_terminal]);
    }
}

void PredictiveTable::insertFirstSets() {
    for (const auto &non_terminal: non_terminals) {
        const std::set<std::pair<std::string, Production>, CompareFirst> &first_set = computed_first_sets[non_terminal];
        for (const auto &first: first_set) {
            if (isEpsilon(first))
                insertEpsilonAtFollowSet(non_terminal, first.second,
                                         computed_follow_sets[non_terminal]);
            else
                insertProduction(non_terminal, first.first, first.second, PredictiveTableEnum::NOT_EMPTY);
        }
    }
}

bool PredictiveTable::isEpsilon(const std::pair<std::string, const Production> &first) {
    return first.first == EPSILON;
}

void PredictiveTable::insertEpsilonAtFollowSet(const std::string &non_terminal,
                                               const Production &production, const std::set<std::string> &follow_set) {
    for (const auto &follow: follow_set) {
        insertProduction(non_terminal, follow, production, PredictiveTableEnum::NOT_EMPTY);
    }
}

void
PredictiveTable::addSynchAtFollowSetElements(const std::string &non_terminal, const std::set<std::string> &follow_set) {
    for (const auto &follow: follow_set) {
        Production production;
        if (containsKey(non_terminal, follow))
            continue;
        insertProduction(non_terminal, follow, production, PredictiveTableEnum::SYNCHRONIZING);
    }
}

bool PredictiveTable::containsKey(const std::string &non_terminal,
                                  const std::basic_string<char> &follow) {
    return predictive_table.find(CellKey(non_terminal, follow)) != predictive_table.end();
}

CellValue *PredictiveTable::lookUp(const std::string &non_terminal, const std::string &terminal) {
    auto cell_key = new CellKey(non_terminal, terminal);
    if (!containsKey(non_terminal, terminal)) {
        Production production;
        return new CellValue(production, PredictiveTableEnum::EMPTY);
    }
    auto cell_value = predictive_table[*cell_key];
    return cell_value;
}

PredictiveTableEnum PredictiveTable::getCellType(const std::string &non_terminal, const std::string &terminal) {
    auto cell_key = new CellKey(non_terminal, terminal);
    if (containsKey(non_terminal, terminal)) {
        auto cell_value = predictive_table[*cell_key];
        if (cell_value->getPredictiveTableEnum() == PredictiveTableEnum::SYNCHRONIZING)
            return PredictiveTableEnum::SYNCHRONIZING;
        else
            return PredictiveTableEnum::NOT_EMPTY;
    }
    return PredictiveTableEnum::EMPTY;
}

bool PredictiveTable::hasProduction(const std::string &non_terminal, const std::string &terminal) {
    return getCellType(non_terminal, terminal) == PredictiveTableEnum::NOT_EMPTY;
}

bool PredictiveTable::isCellEmpty(const std::string &non_terminal, const std::string &terminal) {
    return getCellType(non_terminal, terminal) == PredictiveTableEnum::EMPTY;
}

bool PredictiveTable::isSynchronizing(const std::string &non_terminal, const std::string &terminal) {
    return getCellType(non_terminal, terminal) == PredictiveTableEnum::SYNCHRONIZING;
}

void PredictiveTable::insertProduction(const std::string &non_terminal, const std::string &terminal,
                                       const Production &production, PredictiveTableEnum predictive_table_enum) {
    auto cell_key = new CellKey(non_terminal, terminal);
    auto cell_value = new CellValue(production, predictive_table_enum);
    if (containsKey(non_terminal, terminal)) {
        std::cout << "\nGrammar isn't LL(1)\n";
        printConflict(non_terminal, terminal, production);
        return;
    }
    predictive_table[*cell_key] = cell_value;
}

void PredictiveTable::printPredictiveTable() {
    for (const auto &element: predictive_table) {
        std::cout << element.first.getNonTerminal() << ", " << element.first.getTerminal();

        switch (element.second->getPredictiveTableEnum()) {
            case PredictiveTableEnum::EMPTY:
                std::cout << " --> EMPTY\n";
                break;
            case PredictiveTableEnum::SYNCHRONIZING:
                std::cout << " --> SYNCHRONIZING\n";
                break;
            case PredictiveTableEnum::NOT_EMPTY:
                std::cout << " --> ";
                for (const auto &i: element.second->getProduction().productions[0])
                    std::cout << i;
                std::cout << "\n";
                break;
            default:
                continue;
        }
    }
}

void
PredictiveTable::printConflict(const std::string &non_terminal, const std::string &terminal,
                               const Production &production) {
    std::cout << "Conflict at: " << non_terminal << ", " << terminal << " --> ";

    for (const auto &i: production.productions[0])
        std::cout << i << " ";
    std::cout << "\n";

    std::cout << "Preferred production: ";
    for (const auto &i: predictive_table[CellKey(non_terminal, terminal)]->getProduction().productions[0])
        std::cout << i << " ";
    std::cout << "\n";
}
