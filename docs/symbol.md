```
AST:
int x = 3 + y;

AssignExpr:
left:
  symbol:
    ident: x;
    scope: -> scope in symtab;
    type: type;
right:
  BinaryExpr:
    op: add
    left:
      Const: 3
    right:
      symbol:
        ident: y;
        scope: -> scope in symtab;
        type: type;
```
