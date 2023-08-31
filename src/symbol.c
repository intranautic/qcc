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

  return hashmap_insert(
    table->current->lookup,
    &(Entry) {
      .key = (const char *)symbol->ident->value,
      .value = symbol
    }
  );
}

int symtab_remove(Symtab* table, Symbol* symbol) {
  if (!table || !symbol)
    return -1;

  return hashmap_remove(
    table->current->lookup,
    (const char *)symbol->ident->value
  );
}

Symbol* scope_lookup(Scope* scope, Token* ident) {
  if (!scope)
    return NULL;

  Entry* result = hashmap_retrieve(scope->lookup, (const char *)ident->value);
  return (result)
    ? (Symbol *)result->value
    : scope_lookup(scope->upref, ident);
}
