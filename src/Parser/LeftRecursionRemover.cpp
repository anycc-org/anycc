#include "Parser/LeftRecursionRemover.h"
#include <algorithm>
#include <iostream>

bool LeftRecursionRemover::isImmediateLR(const std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, std::string non_terminal) {
    if(grammar.find(non_terminal) == grammar.end()) return false;
    const auto& prods = grammar.at(non_terminal); 
    for(const auto& prod : prods) {
        if(prod[0] == non_terminal) return true;
    }
    return false;
}

bool LeftRecursionRemover::removeImmediateLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, std::string non_terminal) {
    if(grammar.find(non_terminal) == grammar.end()) return false;
    auto& prods = grammar.at(non_terminal); 
    for(auto& prod : prods) {
        if(prod[0] == non_terminal) {
           std::vector<std::string> alpha(prod.begin() + 1, prod.end()); 
           
        }
    }
    return false;
}


void LeftRecursionRemover::removeLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar) {
    std::vector<std::string> prev_non_terminals;
    for(auto& kv : grammar) {
        std::string non_terminal = kv.first;
        for(auto& prev_non_terminal : prev_non_terminals) {
            LeftRecursionRemover::substituteRHS(grammar, non_terminal, prev_non_terminal);
        }
        if(isImmediateLR(grammar, non_terminal)) {
            LeftRecursionRemover::removeImmediateLR(grammar, non_terminal);
        }
        prev_non_terminals.push_back(non_terminal);
    }
}


bool LeftRecursionRemover::substituteRHS(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, std::string lhs_non_terminal, std::string rhs_non_terminal) {
    auto& src_prods = grammar.at(rhs_non_terminal);
    auto& dist_prods = grammar.at(lhs_non_terminal);
    std::vector<std::vector<std::string>> new_dist_prods;
    for(auto dist_prod : dist_prods) {
        for(size_t i = 0; i < src_prods.size(); i++) {
            std::vector<std::string> new_dist_prod;
            if(std::find(dist_prod.begin(), dist_prod.end(), rhs_non_terminal) == dist_prod.end()) {
                new_dist_prods.push_back(dist_prod);
                break;
            }
            for(size_t j = 0; j < dist_prod.size(); j++) {
                if(dist_prod[j] == rhs_non_terminal) {
                    for(auto src_prod : src_prods[i]) {
                        new_dist_prod.push_back(src_prod);
                    }
                }
                else {
                    new_dist_prod.push_back(dist_prod[j]);
                }
            }
            new_dist_prods.push_back(new_dist_prod);
        }
    }
    grammar.at(lhs_non_terminal) = new_dist_prods;
    return true;
}