macros and directives are the first pass within the frontend phase
the expanding of such macros is done during lexical analysis, which is
done during syntax/parsing phase. expansion will be handled in lexer_get.

since directives change the state of the lexer (such as #include) we should
not decouple them.
