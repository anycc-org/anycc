#include "Parser/CellKey.h"

CellKey::CellKey(const std::string &non_terminal, const std::string &terminal) {
    this->non_terminal = non_terminal;
    this->terminal = terminal;
}

CellKey::~CellKey() {

}

const std::string &CellKey::getNonTerminal() const {
    return non_terminal;
}

const std::string &CellKey::getTerminal() const {
    return terminal;
}
