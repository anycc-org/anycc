#pragma once
#include <iostream>
#include <vector>
#include <Lex/State.h>



class TransitionDiagram {
public:
    TransitionDiagram() = default;
    ~TransitionDiagram() = default;

    State* lookup(State* state, char input);


private:
    std::vector<State*> states;
    std::vector<char> inputs;


};