# First and Follow Sets

## First Sets

| Non-Terminal | First Set |
| ------------ | --------- |
| TERM | `(` `id` `num`  |
| TERM" | `Epsilon` `mulop`  |
| STATEMENT_LIST" | `Epsilon` `float` `id` `if` `int` `while`  |
| ASSIGNMENT | `id`  |
| WHILE | `while`  |
| PRIMITIVE_TYPE | `float` `int`  |
| DECLARATION | `float` `int`  |
| SIGN | `addop`  |
| EXPRESSION | `(` `addop` `id` `num`  |
| EXPRESSION" | `Epsilon` `relop`  |
| IF | `if`  |
| FACTOR | `(` `id` `num`  |
| METHOD_BODY | `float` `id` `if` `int` `while`  |
| SIMPLE_EXPRESSION | `(` `addop` `id` `num`  |
| SIMPLE_EXPRESSION" | `Epsilon` `addop`  |
| STATEMENT | `float` `id` `if` `int` `while`  |
| STATEMENT_LIST | `float` `id` `if` `int` `while`  |

## Follow Sets

| Non-Terminal | Follow Set |
| ------------ | ---------- |
| SIGN | `(` `id` `num`  |
| WHILE | `$` `float` `id` `if` `int` `while` `}`  |
| PRIMITIVE_TYPE | `id`  |
| IF | `$` `float` `id` `if` `int` `while` `}`  |
| METHOD_BODY | `$`  |
| ASSIGNMENT | `$` `float` `id` `if` `int` `while` `}`  |
| STATEMENT | `$` `float` `id` `if` `int` `while` `}`  |
| TERM" | `)` `;` `addop` `relop`  |
| STATEMENT_LIST" | `$`  |
| TERM | `)` `;` `addop` `relop`  |
| SIMPLE_EXPRESSION" | `)` `;` `relop`  |
| STATEMENT_LIST | `$`  |
| DECLARATION | `$` `float` `id` `if` `int` `while` `}`  |
| EXPRESSION | `)` `;`  |
| EXPRESSION" | `)` `;`  |
| FACTOR | `)` `;` `addop` `mulop` `relop`  |
| SIMPLE_EXPRESSION | `)` `;` `relop`  |
