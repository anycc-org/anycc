# Symbol Table

- [Overview](#overview)
    - [Singleton Design](#singleton-design)
    - [SymbolTableEntry Structure](#symboltableentry-structure)
- [Usage](#usage)
    - [Inserting Entries](#inserting-entries)
    - [Retrieving Entries](#retrieving-entries)
    - [Generating Markdown Table](#generating-markdown-table)
- [Example](#example)

## Overview

### Singleton Design

The Symbol Table is implemented as a Singleton, ensuring that there is only one instance of the table throughout the
compiler's execution. This design choice simplifies access to the symbol table and ensures consistent handling of token
entries.

### SymbolTableEntry Structure

The `SymbolTableEntry` structure encapsulates the information associated with each token in the symbol table. The fields
include:

- `token_name`: The name of the token.
- `token_type`: The type of the token.
- `token_id`: The identifier of the token.
- `line_number`: The line number where the token is found.
- `column_number`: The column number where the token is found.

## Usage

### Inserting Entries

To insert a new entry into the Symbol Table, use the `insertEntry` method. This method takes the word, token name, token
ID, line number, and column number as parameters. If an entry with the same word already exists, the new entry will not
be inserted.

```cpp
SymbolTable::getInstance().insertEntry(word, token_name, token_id, line_number, column_number);
```

### Retrieving Entries

To retrieve the entry associated with a specific lexeme, use the `getEntry` method. This method returns a pointer to
the `SymbolTableEntry` structure for the given lexeme.

```cpp
SymbolTableEntry* entry = SymbolTable::getInstance().getEntry(lexeme);
```

### Generating Markdown Table

The `generateMarkdownTable` method allows you to create a markdown representation of the symbol table. This table
includes columns for the key (lexeme), value (token name), line number, and column number.

```cpp
SymbolTable::getInstance().generateMarkdownTable("output/SymbolTable.md");
```

## Example

Here is an example of using the Symbol Table within the AnyCC Compiler:

```cpp
#include "SymbolTable.h"

// Inserting an entry
std::string word = "example";
std::string token_name = "identifier";
int token_id = 1;
int line_number = 5;
int column_number = 10;

SymbolTable::getInstance().insertEntry(word, token_name, token_id, line_number, column_number);

// Retrieving an entry
SymbolTableEntry* entry = SymbolTable::getInstance().getEntry(word);

// Generating Markdown Table
SymbolTable::getInstance().generateMarkdownTable("output/SymbolTable.md");
```

This example demonstrates the basic operations of inserting, retrieving, and generating a Markdown table using the
Symbol Table in the AnyCC Compiler.