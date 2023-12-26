Parsing input tokens...

| Stack  | Current Token | Output                 |
|--------|---------------|------------------------|
| METHOD_BODY | int | 
| STATEMENT_LIST | int | 
| STATEMENT | int | 
| DECLARATION | int | 
| PRIMITIVE_TYPE | int | 
| int | int | match int |
| id | id | match id |
| ; | ; | match ; |
| STATEMENT_LIST" | id | 
| STATEMENT | id | 
| ASSIGNMENT | id | 
| id | id | match id |
| assign | assign | match assign |
| EXPRESSION | num | 
| SIMPLE_EXPRESSION | num | 
| TERM | num | 
| FACTOR | num | 
| num | num | match num |
| TERM" | ; | 
| SIMPLE_EXPRESSION" | ; | 
| EXPRESSION" | ; | 
| ; | ; | match ; |
| STATEMENT_LIST" | if | 
| STATEMENT | if | 
| IF | if | 
| if | if | match if |
| ( | ( | match ( |
| EXPRESSION | id | 
| SIMPLE_EXPRESSION | id | 
| TERM | id | 
| FACTOR | id | 
| id | id | match id |
| TERM" | relop | 
| SIMPLE_EXPRESSION" | relop | 
| EXPRESSION" | relop | 
| relop | relop | match relop |
| SIMPLE_EXPRESSION | num | 
| TERM | num | 
| FACTOR | num | 
| num | num | match num |
| TERM" | ) | 
| SIMPLE_EXPRESSION" | ) | 
| ) | ) | match ) |
| { | { | match { |
| STATEMENT | id | 
| ASSIGNMENT | id | 
| id | id | match id |
| assign | assign | match assign |
| EXPRESSION | num | 
| SIMPLE_EXPRESSION | num | 
| TERM | num | 
| FACTOR | num | 
| num | num | match num |
| TERM" | ; | 
| SIMPLE_EXPRESSION" | ; | 
| EXPRESSION" | ; | 
| ; | ; | match ; |
| } | } | match } |
| else | else | match else |
| { | { | match { |
| STATEMENT | id | 
| ASSIGNMENT | id | 
| id | id | match id |
| assign | assign | match assign |
| EXPRESSION | num | 
| SIMPLE_EXPRESSION | num | 
| TERM | num | 
| FACTOR | num | 
| num | num | match num |
| TERM" | ; | 
| SIMPLE_EXPRESSION" | ; | 
| EXPRESSION" | ; | 
| ; | ; | match ; |
| } | } | match } |
| STATEMENT_LIST" | $ | 
| $ | $ | match $ |
