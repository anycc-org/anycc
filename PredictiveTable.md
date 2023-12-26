| **Non-Terminal** | **$** | **(** | **)** | **+** | **-** | **;** | **addop** | **float** | **id** | **if** | **int** | **mulop** | **num** | **relop** | **while** | **}** |
|------------------|------------|------------|------------|------------|------------|------------|------------|------------|------------|------------|------------|------------|------------|------------|------------|------------|
| **ASSIGNMENT** | `Synch` | `Synch` | `id assign EXPRESSION ;` | `Synch` | `Synch` | `Synch` | `Synch` |
| **DECLARATION** | `Synch` | `PRIMITIVE_TYPE id ;` | `Synch` | `Synch` | `PRIMITIVE_TYPE id ;` | `Synch` | `Synch` |
| **EXPRESSION** | `SIMPLE_EXPRESSION EXPRESSION"` | `Synch` | `SIMPLE_EXPRESSION EXPRESSION"` | `SIMPLE_EXPRESSION EXPRESSION"` | `Synch` | `SIMPLE_EXPRESSION EXPRESSION"` | `SIMPLE_EXPRESSION EXPRESSION"` |
| **EXPRESSION"** | `Epsilon` | `Epsilon` | `relop SIMPLE_EXPRESSION` |
| **FACTOR** | `( EXPRESSION )` | `Synch` | `Synch` | `Synch` | `id` | `Synch` | `num` | `Synch` |
| **IF** | `Synch` | `Synch` | `Synch` | `if ( EXPRESSION ) { STATEMENT } else { STATEMENT }` | `Synch` | `Synch` | `Synch` |
| **METHOD_BODY** | `Synch` | `STATEMENT_LIST` | `STATEMENT_LIST` | `STATEMENT_LIST` | `STATEMENT_LIST` | `STATEMENT_LIST` |
| **PRIMITIVE_TYPE** | `float` | `Synch` | `int` |
| **SIGN** | `Synch` | `+` | `-` | `Synch` | `Synch` |
| **SIMPLE_EXPRESSION** | `TERM SIMPLE_EXPRESSION"` | `Synch` | `SIGN TERM SIMPLE_EXPRESSION"` | `SIGN TERM SIMPLE_EXPRESSION"` | `Synch` | `TERM SIMPLE_EXPRESSION"` | `TERM SIMPLE_EXPRESSION"` | `Synch` |
| **SIMPLE_EXPRESSION"** | `Epsilon` | `Epsilon` | `addop TERM SIMPLE_EXPRESSION"` | `Epsilon` |
| **STATEMENT** | `Synch` | `DECLARATION` | `ASSIGNMENT` | `IF` | `DECLARATION` | `WHILE` | `Synch` |
| **STATEMENT_LIST** | `Synch` | `STATEMENT STATEMENT_LIST"` | `STATEMENT STATEMENT_LIST"` | `STATEMENT STATEMENT_LIST"` | `STATEMENT STATEMENT_LIST"` | `STATEMENT STATEMENT_LIST"` |
| **STATEMENT_LIST"** | `Epsilon` | `STATEMENT STATEMENT_LIST"` | `STATEMENT STATEMENT_LIST"` | `STATEMENT STATEMENT_LIST"` | `STATEMENT STATEMENT_LIST"` | `STATEMENT STATEMENT_LIST"` |
| **TERM** | `FACTOR TERM"` | `Synch` | `Synch` | `Synch` | `FACTOR TERM"` | `FACTOR TERM"` | `Synch` |
| **TERM"** | `Epsilon` | `Epsilon` | `Epsilon` | `mulop FACTOR TERM"` | `Epsilon` |
| **WHILE** | `Synch` | `Synch` | `Synch` | `Synch` | `Synch` | `while ( EXPRESSION ) { STATEMENT }` | `Synch` |
