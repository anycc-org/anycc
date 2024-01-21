# AnyCC Compiler Project Readme

The AnyCC Compiler is a project aimed at creating a compiler for a custom programming language. The compiler consists of
various components, including a Lexer, Parser, and Symbol Table. This README provides an overview of the entire project,
including how to build and use the compiler.

## Authors

- [Ahmed ElSaeed](https://github.com/Ahmedelsa3eed)
- [Abdelrahman Ahmed Bahaa](https://github.com/AbdelrahmanMosly)
- [Muhammad Ibrahim Elkotb](https://github.com/MuhammadElkotb)
- [Youssef Ali Mohamed Bazina](https://github.com/Bazina)

## Documentation

- [Lexical Analyzer Generator](https://github.com/anycc-org/anycc/blob/ANYCC-54-Write-Documentation/docs/Lexical%20Analyzer%20Generator.md)
- [Parser Generator](https://github.com/anycc-org/anycc/blob/ANYCC-54-Write-Documentation/docs/Parser%20Generator.md)
- [Symbol Table](https://github.com/anycc-org/anycc/blob/ANYCC-54-Write-Documentation/docs/Symbol%20Table.md)

## Project Structure

The project is organized into the following main components:

- **Lex**: The Lexer component responsible for scanning the source code and generating tokens.

- **Parser**: The Parser component responsible for parsing the tokens and building the syntax tree.

- **SymbolTable**: The Symbol Table component responsible for managing information about tokens encountered during
  lexical analysis.

## Building and Running

### Prerequisites

Ensure you have the necessary dependencies installed on your system

- C++ compiler (supporting C++17)
- CMake

### Build Steps

1. Clone the repository

    ```bash
    git clone <repository_url>
    cd AnyCC-Compiler
    ```

2. Create a build directory and navigate into it

    ```bash
    mkdir build
    cd build
    ```

3. Generate build files with CMake

    - On Linux
        ```bash
        cmake -G "Unix Makefiles" ..
        ```
    - On Windows
        ```bash
        cmake -G "MinGW Makefiles" ..
        ```
      or
        ```bash
        cmake -G "Visual Studio 17 2022" ..
        ```

4. Build the project:
      ```bash
      cmake --build .
      ```

### Running the Compiler

The AnyCC Compiler takes three command-line arguments: `<rules_file_name>`, `<cfg_file_name>`,
and `<program_file_name>`. These arguments specify the files containing lexical rules, context-free grammar rules, and
the program to be compiled, respectively.

```bash
./AnyCC <rules_file_name> <cfg_file_name> <program_file_name>
```

### Output

Upon successful execution, the compiler will generate an output folder containing various artifacts, including the
generated Symbol Table in Markdown format.

## Example

Here is an example of how to use the AnyCC Compiler

```bash
./AnyCC ../lex_rules.txt ../cfg_rules.txt ../source_code.txt
```

This example assumes that `lex_rules.txt` contains lexical rules, `cfg_rules.txt` contains context-free grammar rules,
and `source_code.txt` contains the program source code.

## Additional Notes

- The project is actively under development, and improvements and additional features will be added over time.

- For detailed information about each component (Lex, Parser, Symbol Table), refer to their respective README files in
  their corresponding directories.

- Please report any issues or bugs on the project's GitHub repository.

Enjoy using the AnyCC Compiler!