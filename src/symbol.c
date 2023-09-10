#include <stdlib.h>
#include <stdbool.h>

#include "qcc/symbol.h"
#include "qcc/initalloc.h"

static Scope* scope_create(Scope* upref) {
  Scope* scope = (Scope *)malloc(sizeof(Scope));
  scope->lookup = hashmap_create();
  scope->typeref = hashmap_create();
  scope->upref = upref;
  scope->subscope = NULL;
  return scope;
}

// deallocates all children scopes as well
static void scope_destroy(Scope* scope) {
  if (!scope)
    return;

  Scope* tmp=0;
  while (tmp = list_fpop(&scope->subscope)) {
    if (tmp == (Scope *)-1)
      break;
    scope_destroy(tmp); 
  }

  hashmap_destroy(scope->lookup);
  hashmap_destroy(scope->typeref);
  free(scope);
  return;
}

static int scope_typedef(Scope* scope, const char* name, Type* type) {
  return (scope)
    ? hashmap_insert(scope->typeref, &(Entry) {
      .key = name,
      .value = (void *)type
    })
    : -1;
}

/* --- public symbol object api --- */
Symbol* symbol_create(int store, Type* type, Token* ident) {
  Symbol* symbol = (Symbol *)malloc(sizeof(Symbol));
  symbol->storage = store;
  symbol->type = type;
  symbol->ident = ident;
  // assigned scope when installed into symbol table
  symbol->scope = NULL;
}

void symbol_destroy(Symbol* symbol) {
  if (symbol)
    free(symbol);
  return;
}

/* --- public symbol table api --- */
Symtab* symtab_create(void) {
  Symtab* table = (Symtab *)malloc(sizeof(Symtab));
  table->tag = scope_create(NULL);
  table->current = table->global = scope_create(NULL);
  // install predicate types into global scope
  scope_typedef(table->global, "void", pred_void);
  scope_typedef(table->global, "_Bool", pred_bool);
  scope_typedef(table->global, "char", pred_char);
  scope_typedef(table->global, "short", pred_short);
  scope_typedef(table->global, "int", pred_int);
  scope_typedef(table->global, "long", pred_long);
  scope_typedef(table->global, "float", pred_float);
  scope_typedef(table->global, "double", pred_double);
  return table;
}

void symtab_destroy(Symtab* table) {
  if (table) {
    scope_destroy(table->tag);
    scope_destroy(table->global);
    free(table);
  }
  return;
}

void symtab_enter(Symtab* table) {
  if (table) {
    list_fpush(
      &table->current->subscope,
      scope_create(table->current)
    );
    table->current = (Scope *)list_top(table->current->subscope);
  }
  return;
}

void symtab_leave(Symtab* table) {
  if (table)
    table->current = table->current->upref;
  return;
}

int symtab_install(Symtab* table, Symbol* symbol) {
  if (!table || !symbol)
    return -1;

  symbol->scope = table->current;
  return hashmap_insert(
    table->current->lookup,
    &(Entry) {
      .key = symbol->ident->value.ident,
      .value = symbol
    }
  );
}

int symtab_remove(Symtab* table, Symbol* symbol) {
  if (!table || !symbol)
    return -1;

  return hashmap_remove(
    table->current->lookup,
    symbol->ident->value.ident
  );
}

/* lookup an identifier from a given scope */
Symbol* scope_lookup(Scope* scope, Token* ident) {
  if (!scope)
    return NULL;

  Entry* result = hashmap_retrieve(scope->lookup, ident->value.ident);
  return (result)
    ? (Symbol *)result->value
    : scope_lookup(scope->upref, ident);
}

