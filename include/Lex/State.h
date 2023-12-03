#pragma once
#include <unordered_map>
#include <vector>


class State {
public:

    virtual int getStateId() const = 0;
    virtual bool isEndState() const = 0;
    virtual std::unordered_map<char, std::vector<State*>> getTransitions() = 0;;
    void printState() const;

private:

};