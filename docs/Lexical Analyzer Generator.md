# Lexical Analyzer Phase

## Table of Contents

- [Lexical Analyzer Design](#lexical-analyzer-design)
    - [Reading Rules](#reading-rules)
        - [Algorithm Design](#algorithm-design)
    - [Used Data Structures](#used-data-structures)
    - [Assumptions](#assumptions)
    - [RE to NFA](#re-to-nfa)
        - [Algorithm Design](#algorithm-design-1)
    - [Used Data Structures](#used-data-structures-1)
        - [Assumptions](#assumptions-1)
    - [NFA to DFA](#nfa-to-dfa)
        - [Algorithm Design](#algorithm-design-2)
    - [Used Functions](#used-functions)
    - [Used Data Structures](#used-data-structures-2)
    - [Minimize DFA](#minimize-dfa)
        - [Algorithm Design](#algorithm-design-3)
    - [Used Functions](#used-functions-1)
    - [Used Data Structures](#used-data-structures-3)
    - [Program Simulation & Error Recovery](#program-simulation--error-recovery)
        - [Initialization](#initialization)
        - [Tokenization](#tokenization)
        - [State Transition](#state-transition)
        - [Panic Mode Error Recovery](#panic-mode-error-recovery)

## Lexical Analyzer Design

### Reading Rules

#### Algorithm Design

- **File Reading**
    - The `FileReader` reads the input file containing rules and delegates to `InputReader` for rule parsing.
- **Rule Parsing**
    - `InputReader` parses each line of the file and categorizes rules based on their type.
    - The type is determined using regular expression matching against predefined patterns for different rule types.
    - Each rule is further processed based on its type regular expression/definition, keywords, or punctuations.
- **Rule Building**
    - Rules are built and added to the appropriate data structures (`Rules` object) based on their types.
    - Regular expressions and definitions undergo cleaning and concatenation handling using `Utilities`.

### Used Data Structures

- **Token Class**
    - Represents an identified element in the text/code during lexical analysis.
    - Key A string identifying the token.
    - Value A string representing the lexeme or meaning associated with the token.
- **Rules**
    - Stores categorized rules with their types (regular expressions, definitions, keywords, punctuations).
    - Token Vectors `std::vector<Token *>` storing tokens derived from regular expressions and definitions.
    - Keyword and Punctuation Vectors `std::vector<std::string>` storing keywords and punctuation identified in the
      text.
- **SubstringInfo & compareSubstringInfo**
    - Used for identifying substrings and their positions.
- **Non-Terminal Symbols**
    - Stores non-terminal symbols extracted from rules.

#### Assumptions

- Keywords have the highest priorities among the rules.

## RE to NFA

### Algorithm Design

- **NFA Manipulation**
    - Decomposed into multiple functions, each corresponding to a specific operation on NFAs.
    - Functions include basic operations like creating NFA for a character, word, union, concatenation, Kleene star, and
      positive closure.
- **regexToNFA Method**
    - Employs the shunting-yard algorithm to parse and convert a regular expression into an NFA.
    - Handles operator precedence, escaping, and correct handling of symbols, words, and operators.

### Used Data Structures

- **NFAState Class**
    - Represents a state in an automaton with StateId, TokenName, and Transitions.
- **NFA Class**
    - Represents a Non-Deterministic Finite Automaton with StartState, EndState, TokenName, and EndStates.
- **NFAGenerator Class**
    - Maps strings to associated NFAs.

#### Assumptions

- The '-' symbol is assumed to be an operator.
- The space character represents concatenation.

## NFA to DFA

### Algorithm Design

- **Transition Diagram Creation**
    - A transition diagram is created from the NFA using its start state, end states, rules tokens, and rules tokens
      priority.
- **DFA Conversion**
    - `DeterministicTransitionDiagramMinimizer` is used to convert the diagram into a DFA using the Subset Construction
      Method.

### Used Functions

- **fillTable**
    - Creates a transition diagram from the NFA, filling up the table with states and their transitions on corresponding
      inputs.
- **getAllNextStates**
    - Concatenates all next states given a specific input and a current state.
- **getRecursiveEpsilonClosure**
    - Gets epsilon closure of a given state.
- **subsetConstruction**
    - Does subset construction on the diagram, returning a DFA diagram.

### Used Data Structures

- **TransitionDiagram Class**
    - Encapsulates an FSM Diagram with maps to map from the current state to next states using inputs given.

## Minimize DFA

### Algorithm Design

- **DFA Minimization**
    - `TransitionDiagramMinimizer` is used to convert the diagram into a minimized DFA using the Equivalence Theorem
      Method.

### Used Functions

- **constructEquivalenceTable**
    - Constructs the equivalence tables using a given set of states.
- **constructNewEquivalenceSets**
    - Breaks the given set of states into a new set using the given equivalence table.

### Used Data Structures

- **TransitionDiagram Class**
    - Encapsulates an FSM Diagram with maps to map from the current state to next states using inputs given.

## Program Simulation & Error Recovery

### Initialization

- Initialize the Analyzer with the program file name, start state, transition diagram, and an empty symbol table.
- Get a set of dead states from the transition diagram.

### Tokenization

- Read the program file character by character.
- For each character, check for end-of-line or space.
- If found, validate the current buffer as a token, reset the buffer, and reset the state to the start state.
- If it's not a space or newline, proceed to process the character.

### State Transition

- Based on the current character, determine the next state.
- Continue traversing the DFA until an accepted or error state is reached.
- If an accepted state is reached, store it and continue trying to get the longest prefix “Maximal Munch”.
- If an error state is reached, attempt error recovery.

### Panic Mode Error Recovery

- On encountering an error state
    - Report the bad token if there's no previously accepted token.
    - Accept the current buffer as a token if there's a previously accepted token.
    - Try to recover from the error state by traversing the buffer separately.