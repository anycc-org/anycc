#ifndef ANYCC_TRANSITIONDIAGRAMMINIMIZER_H
#define ANYCC_TRANSITIONDIAGRAMMINIMIZER_H

#include "Lex/NFAState.h"
#include "Lex/TransitionDiagram.h"

class TransitionDiagramMinimizer {
public:
    TransitionDiagramMinimizer() = default;

    ~TransitionDiagramMinimizer() = default;

    /**
     * @brief minimize
      the Transition Diagram and return a pointer to the Transition Diagram
     * @warning in case of inplace was false, the deletion of the returned Transition Diagram Pointer is the responsibility of
     the caller  
     * @param inplace if true the same pointer passed will be returned, no new object created, otherwise, a new Transition Diagram will be created. 
     * @return TransitionDiagram* Transition Diagram pointer , see warning
    */
    static TransitionDiagram *minimize(TransitionDiagram *transition_diagram, bool inplace = true);


private:
    static TransitionDiagram *minimizeInplace(TransitionDiagram *transition_diagram);

    static std::unordered_map<const NFAState *, std::vector<size_t>>
    constructEquivalenceTable(TransitionDiagram *transition_diagram, std::vector<std::set<const NFAState *>> &sets);

    static long long getSetIndex(const NFAState *state, std::vector<std::set<const NFAState *>> &sets);

    static std::vector<std::set<const NFAState *>> constructNewEquivalenceSets(std::set<const NFAState *> &set,
                                                                               std::unordered_map<const NFAState *, std::vector<size_t>> &table);

    static std::set<const NFAState *>
    extractNewMergedStatesFromOld(const NFAState *state, std::vector<std::set<const NFAState *>> states);
};

#endif //ANYCC_TRANSITIONDIAGRAMMINIMIZER_H