#ifndef ANYCC_CFGREADER_H
#define ANYCC_CFGREADER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <iostream>


class CFGReader {
public:
    static std::string start_symbol;

    /**
       * @brief Parse the CFG input file and generate a grammar representation.
       *
       * This function reads a CFG input file and generates a grammar representation as an unordered_map with non-terminals as keys and vectors of vectors of strings as values.
       *
       * @param filename The path to the CFG input file.
       * @return An unordered_map representing the grammar parsed from the file.
     */
    static std::unordered_map<std::string, std::vector<std::vector<std::string>>>
    parseCFGInput(const std::string &filename);
};

#endif // ANYCC_CFGREADER_H
