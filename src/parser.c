#include "qcc/parser.h"
#include "qcc/initalloc.h"

/* --- statement parser --- */
static Node* parse_stmt(Parser* parser) {}
/* --- declaration parser --- */
static Node* parse_decl(Parser* parser) {}
/* --- expression parser --- */
static Node* parse_primary_expr(Parser* parser);
static Node* parse_postfix_expr(Parser* parser);
static Node* parse_unary_expr(Parser* parser);
static Node* parse_binary_expr(Parser* parser);
static Node* parse_cond_expr(Parser* parser);
static Node* parse_assign_expr(Parser* parser);
static Node* parse_expr(Parser* parser);

/* --- public parser api --- */
Parser* parser_create(Lexer* lexer, Symtab* table) {}
void parser_destroy(Parser* parser) {}

Node* parser_run(Parser* parser) {}
