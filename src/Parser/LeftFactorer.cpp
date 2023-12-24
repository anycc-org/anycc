#include "Parser/LeftFactorer.h"
#include "constants.h"
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <map>
#include <iostream>
#include <utility>

std::unordered_map<std::string, std::vector<std::vector<std::string>>> LeftFactorer::leftFactor(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar) {
    auto new_grammar = grammar;
    std::stack<std::string> to_be_factored;
    for(const auto& kv : new_grammar) {
        to_be_factored.push(kv.first);
    } 
    while(!to_be_factored.empty()) {
        std::string current_non_terminal = to_be_factored.top();
        to_be_factored.pop();
        size_t factored_count = 1;
        std::string new_non_terminal = LeftFactorer::leftFactorProd(new_grammar, current_non_terminal, factored_count);
        while(new_non_terminal != current_non_terminal) {
            factored_count++;
            to_be_factored.push(new_non_terminal);
            new_non_terminal = LeftFactorer::leftFactorProd(new_grammar, current_non_terminal, factored_count);
        }
    }
    return new_grammar;   
}


std::pair<std::vector<std::string>, std::set<std::vector<std::string>>> LeftFactorer::getLongestCommonPrefix(const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string non_terminal) {
    std::map<std::vector<std::string>, std::set<std::vector<std::string>>> prefix_to_rhs_map;
    auto& prods = grammar.at(non_terminal);
    size_t max_len = 0;
    for(const auto& prod : prods) {
        max_len = std::max(prod.size(), max_len);
    }
    std::vector<std::string> common_prefix;
    size_t max_rhs_len_prefix = 0;
    for(size_t i = 0; i < max_len; i++) {
        for(const auto& prod : prods) {
            std::vector<std::string> prefix;
            for(size_t j = 0; j <= i && j < prod.size(); j++) {
                prefix.push_back(prod[j]);
            }    
            if(!prefix.empty()) {
                if(prefix_to_rhs_map.find(prefix) == prefix_to_rhs_map.end()) {
                    prefix_to_rhs_map[prefix] = std::set<std::vector<std::string>>();
                }
                prefix_to_rhs_map.at(prefix).insert(prod);
                size_t current_rhs_prefix_size = prefix_to_rhs_map.at(prefix).size();
                if(current_rhs_prefix_size > 1) {
                    if(max_rhs_len_prefix < current_rhs_prefix_size) {
                        common_prefix = prefix;
                        max_rhs_len_prefix = prefix_to_rhs_map.at(prefix).size();
                    }
                    else if(max_rhs_len_prefix == current_rhs_prefix_size) {
                        if(common_prefix.size() < prefix.size()) {
                            common_prefix = prefix;
                            max_rhs_len_prefix = prefix_to_rhs_map.at(prefix).size();
                        }
                    }
                }
            }
        }
    }
    if(common_prefix.empty()) return {};
    if(prefix_to_rhs_map.find(common_prefix) == prefix_to_rhs_map.end()) return {};
    return std::pair<std::vector<std::string>, std::set<std::vector<std::string>>>(common_prefix, prefix_to_rhs_map.at(common_prefix));
}

std::string LeftFactorer::leftFactorProd(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string non_terminal, size_t factored_count) {
    auto common_prefix_pair = LeftFactorer::getLongestCommonPrefix(grammar, non_terminal);
    auto common_prefix = common_prefix_pair.first;
    if(common_prefix.empty()) return non_terminal;
    auto common_prefix_betas = common_prefix_pair.second;
    std::string new_non_terminal = non_terminal;
    for(size_t i = 0; i < factored_count; i++) {
        new_non_terminal += NEW_NT_SYMBOL;
    }
    auto old_prods = grammar.at(non_terminal);
    std::vector<std::vector<std::string>> new_prods;
    new_prods.push_back(std::vector<std::string>());
    for(const auto& str : common_prefix) {
        new_prods[0].push_back(str);
    }
    new_prods[0].push_back(new_non_terminal);
    for(auto& prod : old_prods) {
        if(common_prefix_betas.find(prod) == common_prefix_betas.end()) {
            new_prods.push_back(prod);
        }
    }
    grammar.at(non_terminal) = new_prods;
    std::vector<std::vector<std::string>> new_non_terminal_prods;
    for(const auto& gamma : common_prefix_betas) {
        std::vector<std::string> new_prod;
        for(size_t j = common_prefix.size(); j < gamma.size(); j++) {
            new_prod.push_back(gamma[j]);
        }
        if(!new_prod.empty()) {
            new_non_terminal_prods.push_back(new_prod);
        }
        else {
            new_non_terminal_prods.push_back(std::vector<std::string>{EPSILON});
        }
    }
    grammar[new_non_terminal] = new_non_terminal_prods;
    return new_non_terminal;
}
