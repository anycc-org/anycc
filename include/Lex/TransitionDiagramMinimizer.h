#pragma once
#include "Lex/NFAState.h"
#include <Lex/TransitionDiagram.h>

class TransitionDiagramMinimizer {
public:
    TransitionDiagramMinimizer() = default;
    ~TransitionDiagramMinimizer() = default;

    /**
     * @brief minimize
      the Transition Diagram and return a pointer to the Transition Diagram
     * @warning incase of inplace was false, the deletion of the returned Transition Diagram Pointer is the responsibilty of 
     the caller  
     * @param inplace if true the same pointer passed will be returned, no new object created, otherwise, a new Transition Diagram will be created. 
     * @return TransitionDiagram* Transition Diagram pointer , see warning
    */
    TransitionDiagram* minimize(TransitionDiagram* transdig, bool inplace=true);


private:
    TransitionDiagram* minimizeInplace(TransitionDiagram* transdig);
    std::unordered_map<const NFAState*, std::vector<size_t>> constructEquivelanceTable(TransitionDiagram* transdig, std::vector<std::set<const NFAState*>>& sets); 
    long long getSetIndex(const NFAState* state, std::vector<std::set<const NFAState*>>& sets);
    std::vector<std::set<const NFAState*>> constructNewEqivelanceSets(std::set<const NFAState*>& set, std::unordered_map<const NFAState*, std::vector<size_t>>& table); 
    std::set<const NFAState*> extractNewMergedStatesFromOld(const NFAState* state, std::vector<std::set<const NFAState*>> states);
};