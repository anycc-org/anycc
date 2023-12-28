#ifndef ANYCC_DETERMINISTIC_TRANSITION_DIAGRAM_CREATOR_H
#define ANYCC_DETERMINISTIC_TRANSITION_DIAGRAM_CREATOR_H

#include "Lex/TransitionDiagram.h"

class DeterministicTransitionDiagramCreator {
public:
    DeterministicTransitionDiagramCreator() = default;

    ~DeterministicTransitionDiagramCreator() = default;

    /**
     * @brief do subset construction on
      the Transition Diagram and return a pointer to the  Transition Diagram
     * @warning in case of inplace was false, the deletion of the returned Transition Diagram Pointer is the responsibility of
     the caller  
     * @param inplace if true the same pointer passed will be returned, no new object created, otherwise, a new Transition Diagram will be created. 
     * @return TransitionDiagram* Transition Diagram pointer , see warning
     */
    static TransitionDiagram *subsetConstruction(TransitionDiagram *transition_diagram, bool inplace = true);

private:
    static TransitionDiagram *subsetConstructionInplace(TransitionDiagram *transition_diagram);
};

#endif //ANYCC_DETERMINISTIC_TRANSITION_DIAGRAM_CREATOR_H