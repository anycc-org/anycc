*Parsing input tokens...*

| Stack Top | Current Token | Output                 |
|-----------|---------------|------------------------|
| METHOD_BODY | int | METHOD_BODY &#8594; STATEMENT_LIST 
| STATEMENT_LIST | int | STATEMENT_LIST &#8594; STATEMENT STATEMENT_LIST" 
| STATEMENT | int | STATEMENT &#8594; DECLARATION 
| DECLARATION | int | DECLARATION &#8594; PRIMITIVE_TYPE id ; 
| PRIMITIVE_TYPE | int | PRIMITIVE_TYPE &#8594; int 
| int | int | match ``int`` |
| id | id | match ``id`` |
| ; | ; | match ``;`` |
| STATEMENT_LIST" | id | STATEMENT_LIST" &#8594; STATEMENT STATEMENT_LIST" 
| STATEMENT | id | STATEMENT &#8594; ASSIGNMENT 
| ASSIGNMENT | id | ASSIGNMENT &#8594; id assign EXPRESSION ; 
| id | id | match ``id`` |
| assign | assign | match ``assign`` |
| EXPRESSION | num | EXPRESSION &#8594; SIMPLE_EXPRESSION EXPRESSION" 
| SIMPLE_EXPRESSION | num | SIMPLE_EXPRESSION &#8594; TERM SIMPLE_EXPRESSION" 
| TERM | num | TERM &#8594; FACTOR TERM" 
| FACTOR | num | FACTOR &#8594; num 
| num | num | match ``num`` |
| TERM" | ; | TERM" &#8594; Epsilon 
| SIMPLE_EXPRESSION" | ; | SIMPLE_EXPRESSION" &#8594; Epsilon 
| EXPRESSION" | ; | EXPRESSION" &#8594; Epsilon 
| ; | ; | match ``;`` |
| STATEMENT_LIST" | if | STATEMENT_LIST" &#8594; STATEMENT STATEMENT_LIST" 
| STATEMENT | if | STATEMENT &#8594; IF 
| IF | if | IF &#8594; if ( EXPRESSION ) { STATEMENT } else { STATEMENT } 
| if | if | match ``if`` |
| ( | ( | match ``(`` |
| EXPRESSION | id | EXPRESSION &#8594; SIMPLE_EXPRESSION EXPRESSION" 
| SIMPLE_EXPRESSION | id | SIMPLE_EXPRESSION &#8594; TERM SIMPLE_EXPRESSION" 
| TERM | id | TERM &#8594; FACTOR TERM" 
| FACTOR | id | FACTOR &#8594; id 
| id | id | match ``id`` |
| TERM" | relop | TERM" &#8594; Epsilon 
| SIMPLE_EXPRESSION" | relop | SIMPLE_EXPRESSION" &#8594; Epsilon 
| EXPRESSION" | relop | EXPRESSION" &#8594; relop SIMPLE_EXPRESSION 
| relop | relop | match ``relop`` |
| SIMPLE_EXPRESSION | num | SIMPLE_EXPRESSION &#8594; TERM SIMPLE_EXPRESSION" 
| TERM | num | TERM &#8594; FACTOR TERM" 
| FACTOR | num | FACTOR &#8594; num 
| num | num | match ``num`` |
| TERM" | ) | TERM" &#8594; Epsilon 
| SIMPLE_EXPRESSION" | ) | SIMPLE_EXPRESSION" &#8594; Epsilon 
| ) | ) | match ``)`` |
| { | { | match ``{`` |
| STATEMENT | id | STATEMENT &#8594; ASSIGNMENT 
| ASSIGNMENT | id | ASSIGNMENT &#8594; id assign EXPRESSION ; 
| id | id | match ``id`` |
| assign | assign | match ``assign`` |
| EXPRESSION | num | EXPRESSION &#8594; SIMPLE_EXPRESSION EXPRESSION" 
| SIMPLE_EXPRESSION | num | SIMPLE_EXPRESSION &#8594; TERM SIMPLE_EXPRESSION" 
| TERM | num | TERM &#8594; FACTOR TERM" 
| FACTOR | num | FACTOR &#8594; num 
| num | num | match ``num`` |
| TERM" | ; | TERM" &#8594; Epsilon 
| SIMPLE_EXPRESSION" | ; | SIMPLE_EXPRESSION" &#8594; Epsilon 
| EXPRESSION" | ; | EXPRESSION" &#8594; Epsilon 
| ; | ; | match ``;`` |
| } | } | match ``}`` |
| else | else | match ``else`` |
| { | { | match ``{`` |
| STATEMENT | id | STATEMENT &#8594; ASSIGNMENT 
| ASSIGNMENT | id | ASSIGNMENT &#8594; id assign EXPRESSION ; 
| id | id | match ``id`` |
| assign | assign | match ``assign`` |
| EXPRESSION | num | EXPRESSION &#8594; SIMPLE_EXPRESSION EXPRESSION" 
| SIMPLE_EXPRESSION | num | SIMPLE_EXPRESSION &#8594; TERM SIMPLE_EXPRESSION" 
| TERM | num | TERM &#8594; FACTOR TERM" 
| FACTOR | num | FACTOR &#8594; num 
| num | num | match ``num`` |
| TERM" | ; | TERM" &#8594; Epsilon 
| SIMPLE_EXPRESSION" | ; | SIMPLE_EXPRESSION" &#8594; Epsilon 
| EXPRESSION" | ; | EXPRESSION" &#8594; Epsilon 
| ; | ; | match ``;`` |
| } | } | match ``}`` |
| STATEMENT_LIST" | $ | STATEMENT_LIST" &#8594; Epsilon 
| $ | $ | match ``$`` |
