#include <stdlib.h>
#include <stdbool.h>

#include "qcc/symbol.h"
#include "qcc/initalloc.h"

static Scope* scope_create(Scope* upref) {
  Scope* scope = (Scope *)malloc(sizeof(Scope));
  scope->lookup = hashmap_create();
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
  free(scope);
  return;
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
      .key = symbol->ident->value.identifier,
      .value = symbol
    }
  );
}

int symtab_remove(Symtab* table, Symbol* symbol) {
  if (!table || !symbol)
    return -1;

  return hashmap_remove(
    table->current->lookup,
    symbol->ident->value.identifier
  );
}

/* lookup an identifier from a given scope */
Symbol* scope_lookup(Scope* scope, Token* ident) {
  if (!scope)
    return NULL;

  Entry* result = hashmap_retrieve(scope->lookup, ident->value.identifier);
  return (result)
    ? (Symbol *)result->value
    : scope_lookup(scope->upref, ident);
}

