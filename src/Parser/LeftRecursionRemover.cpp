#include "Parser/LeftRecursionRemover.h"
#include "constants.h"
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

void LeftRecursionRemover::removeImmediateLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string non_terminal) {
    if(grammar.find(non_terminal) == grammar.end()) return;
    if(!isImmediateLR(grammar, non_terminal)) return;
    std::string new_non_terminal = non_terminal + "`";
    auto& prods = grammar.at(non_terminal); 
    std::vector<std::vector<std::string>> new_prods;
    std::vector<std::vector<std::string>> new_prods_prime;
    for(auto& prod : prods) {
        if(prod[0] != non_terminal) {
            new_prods.push_back(prod); 
        }
        else {
            std::vector<std::string> alpha(prod.begin() + 1, prod.end());
            new_prods_prime.push_back(alpha);
        }
    }
    for(size_t i = 0; i < new_prods.size(); i++) {
        new_prods[i].push_back(new_non_terminal);
    }
    for(size_t i = 0; i < new_prods_prime.size(); i++) {
        new_prods_prime[i].push_back(new_non_terminal);
    }
    new_prods_prime.push_back(std::vector<std::string>{EPSILON});
    grammar.at(non_terminal) = new_prods;
    grammar.insert({new_non_terminal, new_prods_prime});
}


std::unordered_map<std::string, std::vector<std::vector<std::string>>> LeftRecursionRemover::removeLR(std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar) {
    std::vector<std::string> prev_non_terminals;
    std::vector<std::string> ordered_non_terminals;
    auto new_grammar = grammar;
    for(auto& kv : new_grammar) {
        std::cout << kv.first << "\n";
        ordered_non_terminals.push_back(kv.first);
    }
    std::reverse(ordered_non_terminals.begin(), ordered_non_terminals.end());
    for(size_t i = 0; i < ordered_non_terminals.size(); i++) {
        for(size_t j = 0; j < i; j++) {
            bool replace = false;
            for(const auto& prod : new_grammar.at(ordered_non_terminals[j])) {
                if(prod[0] == ordered_non_terminals[i]) {
                    replace = true;
                    break;
                }
            }
            if(replace) {
                LeftRecursionRemover::substituteRHS(new_grammar, ordered_non_terminals[i], ordered_non_terminals[j]);    
            }
        }
        if(isImmediateLR(new_grammar, ordered_non_terminals[i])) {
            LeftRecursionRemover::removeImmediateLR(new_grammar, ordered_non_terminals[i]);
        }

    }
    return new_grammar;
}


void LeftRecursionRemover::substituteRHS(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar, std::string lhs_non_terminal, std::string rhs_non_terminal) {
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
}