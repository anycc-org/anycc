#include "Parser/CellValue.h"

CellValue::CellValue(const Production &production_value, const PredictiveTableEnum predictive_table_enum_value) {
    this->production = production_value;
    this->predictive_table_enum = predictive_table_enum_value;
}

CellValue::~CellValue() {

}

const Production &CellValue::getProduction() const {
    return production;
}

PredictiveTableEnum CellValue::getPredictiveTableEnum() const {
    return predictive_table_enum;
}
