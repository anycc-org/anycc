#pragma once

class State {
public:
    int id;
    bool isAccepting;

    explicit State(int id, bool isAccepting = false);
};
