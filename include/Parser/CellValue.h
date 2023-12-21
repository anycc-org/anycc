#ifndef ANYCC_CELLVALUE_H
#define ANYCC_CELLVALUE_H


#include "Production.h"
#include "ParsingTableEntryType.h"

class CellValue {
public:
    CellValue(const Production &production_value, ParsingTableEntryType predictive_table_enum_value);

    ~CellValue();

    const Production &getProduction() const;

    ParsingTableEntryType getPredictiveTableEnum() const;

private:
    Production production;
    ParsingTableEntryType predictive_table_enum;
};


#endif
