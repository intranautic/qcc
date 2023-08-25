#include <stdlib.h>

#include "qcc/lexer.h"
#include "qcc/logger.h"

#include "qcc/keyword.h"

#define IS_NUMERIC(c) ('0' <= c && c <= '9')
#define IS_ALPHA(c) (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
// first char of identifier cannot be numerical
#define IS_IDENT0(c) (IS_ALPHA(c) || c == '_')
#define IS_IDENT1(c) (IS_ALPHA(c) || c == '_' || IS_NUMERIC(c))

Lexer* lexer_create(List* sources) {
  Lexer* lexer = malloc(sizeof(Lexer));
  lexer->sources = sources;

  lexer->keywords = hashmap_create();
  for (int i = 0; i < KWRD_END; ++i) {
    hashmap_insert(lexer->keywords, &(Entry) {
      .key = kwrd_map[i]->str,
      .value = (void *)kwrd_map[i]
    });
  }

  lexer->macros = hashmap_create();
  // load pre defined macro map
  lexer->cache = NULL;
  return lexer;
}

void lexer_destroy(Lexer* lexer) {
  if (lexer) {
    // lexer->sources passed as pointer, dont dealloc
    hashmap_destroy(lexer->keywords);
    hashmap_destroy(lexer->macros);
    list_destroy(lexer->cache);
    free(lexer);
  }
  return;
}

// TODO: handle directives are implemented in the lexer
static Token* lexer_identifier(Source* source) {
  char* origin = source->cursor;
  while (IS_IDENT1(*source->cursor))
    source->cursor++;

  return token_create(TK_IDENTIFIER, origin, ++source->cursor - origin, 0);
}

static Token* lexer_floatconst(Source* source) {}
static Token* lexer_intconst(Source* source) {}
static Token* lexer_numconst(Source* source) {}

static Token* lexer_charconst(Source* source) {}
static Token* lexer_strconst(Source* source) {}

static int handle_ignore(Source* source) {
  while (*source->cursor) {
    int skip=0;
    switch (*source->cursor) {
      case '\r':
      case '\n':
        source->line++;
      case '\t':
      case ' ':
        source->cursor++;
        skip++;
        break;
      default:
        return skip;
    }
  }
  return 0;
}

static int lexer_comment(Source* source) {
  int delta = 1;
  switch (*source->cursor) {
    case '/':
      while (*source->cursor != '\n')
        source->cursor++;
      source->cursor++;
      return delta;
    case '*':
      source->cursor++;
      while (*source->cursor) {
        if (*source->cursor == '\n')
          delta++;

        if (*source->cursor == '*') {
          if (*++source->cursor == '/') {
            source->cursor++;
            break;
          }
        }
        source->cursor++;
      }
      return delta;
  }
  return -1;
}

Token* lexer_get(Lexer* lexer) {
  Source* source;
  if (lexer && list_length(lexer->sources) > 0) {
    source = (Source *)lexer->sources->value;
    if (*source->cursor) {
      // skip ignore chars and comments
      handle_ignore(source);
      while (*source->cursor == '/') {
        source->cursor++;
        int delta = lexer_comment(source);
        if (delta == -1) {
          logger_fatal(-1, "Could not tokenize comment %s:%d\n",
            source->path, source->line);
        }
        source->line += delta;
        handle_ignore(source);
      }

      // tokenize identifiers or keywords
      if (IS_IDENT0(*source->cursor)) {
        Token* tok = lexer_identifier(source);
        if (!tok) { 
          logger_fatal(-1, "Could not tokenize identifier %s:%d\n",
            source->path, source->line);
        }

        // if identifier is a keyword, return keyword token instead
        Entry* kwrd_entry =
          hashmap_nretrieve(lexer->keywords, tok->loc, tok->length);

        if (kwrd_entry) {
          tok->kind = TK_KEYWORD;
          tok->value = (Keyword *)kwrd_entry->value;
        }
        return tok;
      }

      // tokenize numeric constants
      if (IS_NUMERIC(*source->cursor))
        return lexer_numconst(source);

    }
  }
  return 0;
}

Token* lexer_peek(Lexer* lexer) {}
int lexer_eat(Lexer* lexer) {}

