#pragma once
#include <Lex/TransitionDiagram.h>

class DeterministicTransitionDiagramCreator {
public:
    DeterministicTransitionDiagramCreator() = default;
    ~DeterministicTransitionDiagramCreator() = default;
    /**
     * @brief do subset construction on
      the Transition Diagram and return a pointer to the  Transition Diagram
     * @warning incase of inplace was false, the deletion of the returned Transition Diagram Pointer is the responsibilty of 
     the caller  
     * @param inplace if true the same pointer passed will be returned, no new object created, otherwise, a new Transition Diagram will be created. 
     * @return TransitionDiagram* Transition Diagram pointer , see warning
     */
    TransitionDiagram* subsetConstruction(TransitionDiagram* transdig, bool inplace=true);

private:
    TransitionDiagram* subsetConstructionInplace(TransitionDiagram* transdig);
};