#ifndef ANYCC_CELLVALUE_H
#define ANYCC_CELLVALUE_H


#include "Production.h"
#include "PredictiveTableEnum.h"

class CellValue {
public:
    CellValue(Production &production_value, PredictiveTableEnum predictive_table_enum_value);

    ~CellValue();

    const Production &getProduction() const;

    PredictiveTableEnum getPredictiveTableEnum() const;

private:
    Production production;
    PredictiveTableEnum predictive_table_enum;
};


#endif
