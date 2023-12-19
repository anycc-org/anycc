#include "Parser/CellValue.h"

CellValue::CellValue(const Production &production_value, const ParsingTableEntryType entry_type) {
    this->production = production_value;
    this->entry_type = entry_type;
}

CellValue::~CellValue() {

}

const Production &CellValue::getProduction() const {
    return production;
}

ParsingTableEntryType CellValue::getPredictiveTableEntryType() const {
    return entry_type;
}
