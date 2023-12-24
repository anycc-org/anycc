#include "Parser/CFGReader.h"
std::string CFGReader::start_symbol;

std::unordered_map<std::string, std::vector<std::vector<std::string>>> CFGReader::parseCFGInput(const std::string& filename) {
  std::ifstream inputFile(filename);
  std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar;

  if (inputFile.is_open()) {

    std::string line;
    std::string currentNonTerminal;

    auto processProduction = [&](const std::string& line) {
      std::vector<std::string> production;
      std::istringstream ss(line);
      std::string token;
      if(grammar.empty())
        start_symbol=currentNonTerminal;
      while (ss >> token) {
        if (token == "|" && production.empty()) {
          continue;
        }
        if (token == "|") {
          // Start a new production for the same non-terminal
          grammar[currentNonTerminal].push_back(production);
          production.clear();
        } else {
          token = std::regex_replace(token, std::regex("'"), "");
          production.push_back(token);
        }
      }
      // Add the last production to the grammar
      grammar[currentNonTerminal].push_back(production);
    };

    while (std::getline(inputFile, line)) {
      // Skip comments and empty lines
      if (line.empty())
        continue;

      std::vector<std::string> production;
      if(line[0] == '#'){// new rule
                            // Find the position of = in the line
        std::regex spacesBeforeArrow("\\s*\\=");
        line = std::regex_replace(line, spacesBeforeArrow, "=");
        size_t arrowPos = line.find("="); // first occurence
        if (arrowPos != std::string::npos) {
          // Extract the non-terminal before ::= as the current non-terminal
          currentNonTerminal = line.substr(2, arrowPos-2); //skip '#' index 0 and ' ' index 1

          // Tokenize the RHS of the production rule using '|'
          line = line.substr(arrowPos + 1); // Move past "="
          processProduction(line);
        }
      }else{
        processProduction(line);
      }
    }

    inputFile.close();
  } else {
    std::cerr << "Unable to open file: " << filename << std::endl;
  }

  return grammar;
}