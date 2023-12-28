#ifndef ANYCC_CELLVALUE_H
#define ANYCC_CELLVALUE_H


#include "Production.h"
#include "ParsingTableEntryType.h"

class CellValue {
public:
    CellValue(const Production &production_value, ParsingTableEntryType entry_type);

    ~CellValue();

    const Production &getProduction() const;

    ParsingTableEntryType getPredictiveTableEntryType() const;

private:
    Production production;
    ParsingTableEntryType entry_type;
};


#endif
