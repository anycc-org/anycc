# Phase 2 Parser Generator

## Table of Contents

- [Reading CFG Rules Modifications](#reading-cfg-rules-modifications)
- [Context-Free Grammar FIRST and FOLLOW Set Rules](#context-free-grammar-first-and-follow-set-rules)
- [Predictive Parsing Table](#predictive-parsing-table)
    - [Interface Methods](#interface-methods)
    - [Algorithm](#algorithm)
    - [Example 1](#example-1)
    - [Example 2](#example-2)
- [Predictive Top-Down Parser and Panic Mode Error Recovery](#predictive-top-down-parser-and-panic-mode-error-recovery)
    - [Predictive Top-Down Parser Algorithm](#predictive-top-down-parser-algorithm)
    - [Panic Mode Error Recovery Algorithm](#panic-mode-error-recovery-algorithm)
- [Used Data Structures](#used-data-structures)
- [Combine Two Phases of Compiler](#combine-two-phases-of-compiler)
    - [Proposed Changes to Lexical Analyzer Generator](#proposed-changes-to-lexical-analyzer-generator)
    - [Proposed Changes to Parser Generator](#proposed-changes-to-parser-generator)
    - [Proposed Changes to PredictiveTopDownParser](#proposed-changes-to-predictivetopdownparser)
    - [Proposed Changes to AnyCC](#proposed-changes-to-anycc)
- [Convert Input grammar To LL(1) Grammar Automatically](#convert-input-grammar-to-ll1-grammar-automatically)

## Reading CFG Rules Modifications

Refactors the `parseCFGInput` function in the `Utilities` class to improve code organization and readability. The
primary changes involve

- Introducing a lambda function `processProduction` to encapsulate common logic for processing production lines.
- Replacing existing production processing logic with calls to the new lambda function.
- Adjusting the handling of new rules (lines starting with '#') to match the new processing approach.
- Ensuring each production is added to the `grammar` map by copying the vector, preventing issues with empty
  productions.
- Adding support for processing multiple lines in the grammar.

## Context-Free Grammar FIRST and FOLLOW Set Rules

### FIRST Set Rules

1. **Terminal Symbol**
    - If `x` is a terminal, then `FIRST(x) = { 'x' }`.

2. **Production Rule with Є (empty string)**
    - If `X -> Є` is a production rule, add Є to `FIRST(X)`.

3. **Production Rule with Non-terminal Symbols**
    - If `X -> Y1 Y2 Y3….Yn` is a production, then `FIRST(X) = FIRST(Y1)`.

4. **Handling Є in FIRST Sets**
    - If `FIRST(Y1)` contains Є, then `FIRST(X) = { FIRST(Y1) – Є } U { FIRST(Y2) }`.
    - If `FIRST(Yi)` contains Є for all `i = 1 to n`, then add Є to `FIRST(X)`.

### FOLLOW Set Rules

1. **Starting Non-Terminal**
    - `FOLLOW(S) = { $ }` where `S` is the starting Non-Terminal.

2. **Between Non-terminals in a Production**
    - If `A -> pBq` is a production, where `p`, `B`, and `q` are any grammar symbols, then everything in `FIRST(q)`
      except Є is in `FOLLOW(B)`.

3. **At the End of a Production**
    - If `A -> pB` is a production, then everything in `FOLLOW(A)` is in `FOLLOW(B)`.

4. **Handling Є in Productions**
    - If `A -> pBq` is a production and `FIRST(q)` contains Є, then `FOLLOW(B)` contains `{ FIRST(q) – Є } U FOLLOW(A)`.

These rules are crucial for constructing predictive parsing tables and resolving parsing conflicts in LL(k) grammars.
They define the behavior of a predictive parser by providing information about the possible expansions and token
predictions at each step in the parsing process.

> **Note** Epsilon can be changed in how it is represented in `constants.h`.

### References

- [GeeksforGeeks - FIRST Set in Syntax Analysis](https://www.geeksforgeeks.org/first-set-in-syntax-analysis/)
- [GeeksforGeeks - FOLLOW Set in Syntax Analysis](https://www.geeksforgeeks.org/follow-set-in-syntax-analysis/)

## Predictive Parsing Table

### Interface Methods

- `lookUp(nonTerminal, terminal)`
    - Returns the production rule for the given non-terminal and terminal.
    - If the given non-terminal and terminal is not present in the table, then returns value contains null productions
      and Enum for the cell if it is `Synch` or `Empty`.
- `getCellType(nonTerminal, terminal)`
    - Returns the type of the cell for the given non-terminal and terminal. The type of the cell can be `Synch`, `Empty`
      or `Non Empty`.
- `hasProduction(nonTerminal, terminal)`
    - Returns true if the given non-terminal and terminal has production rule in the table.
- `isCellEmpty(nonTerminal, terminal)`
    - Returns true if the given non-terminal and terminal is empty.
- `isSynchronizing(nonTerminal, terminal)`
    - Returns true if the given non-terminal and terminal is synchronizing.
- `printPredictiveTable()`
    - Prints the predictive parsing table.

### Algorithm

1. **Input:** First and Follow sets of non-terminals.
2. **Output:** Predictive Parsing Table.
3. **Method**
    - For each production `A -> α` do
        - For each terminal `a` in `FIRST(α)` do
            - Add `A -> α` to `M[A, a]`.
        - If `Є` is in `FIRST(α)` then
            - For each terminal `b` in `FOLLOW(A)` do
                - Add `A -> α` to `M[A, b]`.
        - If `Є` is in `FIRST(α)` and `$` is in `FOLLOW(A)` then
            - Add `A -> α` to `M[A, $]`.
    - If there is more than one production in `M[A, a]` then the grammar is not LL(1) and prefer the existed production.
    - Mark the empty cells as error.
    - Mark the entries `M[A, a]` and `M[A, $]` as synchronizing if `a` is in the follow set of `A`.

### Example 1

#### Consider the following CFG

    S --> i C t S E | a
    E --> e S | Epsilon
    C --> b

#### First and Follow sets

| **Non-Terminal** | **First Set** | **Follow Set** |
|------------------|---------------|----------------|
| **S**            | `a i`         | `$ e`          |
| **E**            | `Epsilon e`   | `$ e`          |
| **C**            | `b`           | `t`            |

#### Predictive Parsing Table

- There

is Conflict at `E, e --> e S` and Preferred production `E, e --> Epsilon` `Grammar not LL(1)`

| **Non-Terminal** | **a** | **b** | **e**     | **i**       | **t**   | **$**     |
|------------------|-------|-------|-----------|-------------|---------|-----------|
| **S**            | `a`   |       | `Synch`   | `i C t S E` |         | `Synch`   |
| **E**            |       |       | `Epsilon` |             |         | `Epsilon` |
| **C**            |       | `b`   |           |             | `Synch` |           |

### Example 2

#### Consider the following CFG

    E --> T E`
    E` --> + T E` | Epsilon
    T --> F T`
    T` --> * F T` | Epsilon
    F --> ( E ) | id

#### First and Follow sets

| **Non-Terminal** | **First Set** | **Follow Set** |
|------------------|---------------|----------------|
| **E**            | `( id`        | `$ )`          |
| **E`**           | `+ Epsilon`   | `$ )`          |
| **T**            | `( id`        | `$ + )`        |
| **T`**           | `* Epsilon`   | `$ + )`        |
| **F**            | `( id`        | `$ * + )`      |

#### Predictive Parsing Table

| **Non-Terminal** | **id**  | **+**     | __*__    | **(**  | **)**     | **$**     |
|------------------|---------|-----------|----------|--------|-----------|-----------|
| **E**            | `T E'`  |           |          | `T E'` | `Synch`   | `Synch`   |
| **E`**           |         | `+ T E'`  |          |        | `Epsilon` | `Epsilon` |
| **T**            | `F T'`  | `Synch`   |          | `F T'` | `Synch`   | `Synch`   |
| **T`**           |         | `Epsilon` | `* F T'` |        | `Epsilon` | `Epsilon` |
| **F**            | `( E )` | `Synch`   | `Synch`  | `id`   | `Synch`   | `Synch`   |

## Predictive Top-Down Parser and Panic Mode Error Recovery

### Predictive Top-Down Parser Algorithm

**parseInputTokens**

- Get next token from lexical.
- While stack is not empty
    - If top is terminal
        - If Top == curr_token: match.
        - elif top == $: handle end of stack.
        - else handle missing terminal.
    - Else if top is non-terminal
        - Cell = M[top, curr_token].
        - If cell is empty: handle empty cell.
        - Elif cell is sync: handle sync cell.
        - Elif cell is valid production: handle valid production.

### Panic Mode Error Recovery Algorithm

- **Handle missing terminal**
    - If top is terminal and does not match with the input symbol: pop stack.

- **Handle Empty cell**
    - If top is non-terminal and M[A, a] is empty: get next token.

- **Handle Sync cell**
    - If top is non-terminal and M[A, a] is sync: pop stack.

- **Handle end of stack**
    - If top == $ and curr_token != $: get next token until $ is encountered.

- **Handle end of input**
    - If curr_token == $ and stack is not empty: pop stack.

Report an error message in each of the previous cases.

## Used Data Structures

- **Parsing stack**
    - Usage: Stack Item is a struct of string and boolean representing the token and is it terminal or not.

- **Leftmost derivation**
    - Structure: It’s a vector of vectors of strings.
    - Usage: It holds the used productions while parsing.


- [x] Create ``PredictiveTopDownParser`` which produces the leftmost derivation for a correct input.
    - It takes the ``PredictiveTable`` and the set of ``non_terminals`` as input.
    - If the input grammar is not LL(1), an appropriate error message will be produced.
    - It's main method is ``processNextToken(token)``  which matches the input symbol with top of stack and applies the
      panic mode error recovery in case of ERROR
- [x] Prints the leftmost derivation as follows
  ![image](https://github.com/anycc-org/anycc/assets/73740339/94d46933-a975-412a-8df8-ccb36e7fb4f6)

## Combine Two Phases of Compiler

### Proposed Changes to Lexical Analyzer Generator

- Add a new method to get next token ``getNextToken()`` which saves the state of the last position in the buffer and
  token so next tokens can be generated.
- Add **position** field to Token class to save the position of the token in the buffer. so can be used to generate
  error messages.
- Return `$` token when EOF is reached.
- Change return type of `acceptToken()`, `maximalMunchWithErrorRecovery` and `addToken` to `Token` instead of `void` so
  the token can be used in the parser.
- Save symbol table in a markdown file `SymbolTable.md`.

### Proposed Changes to Parser Generator

- Add Parser class to parse the tokens generated by the lexical analyzer.
- Save predictive parsing table in a markdown file `PredictiveTable.md`.

### Proposed Changes to PredictiveTopDownParser

- Refactor the ``parseInputTokens()`` function.
- Add the position of the curr_token to the error message while handling **panic mode error recovery**.
- Generating a leftmost derivation markdown into a file called ``LeftmostDerivation.md``
- Generating a leftmost parsing markdown into a file called ``LL1ParsingOutput.md``

### Proposed Changes to AnyCC

- Get file name from command line arguments (e.g. `../input/rules.txt ../input/CFG.txt ../input/program.txt`).
- Use Lex and Parser class to generate tokens and parse them.

## Convert Input grammar To LL(1) Grammar Automatically

- Added **LeftRecursionRemover** class that has **removeLR** method that takes a grammar and returns a new grammar that
  is left recursion free.
- Added **LeftFactorer** class that has **leftFactor** method that takes a grammar and returns a new left factored
  grammar.