#include <stdlib.h>

#include "qcc/lexer.h"
#include "qcc/logger.h"
#include "qcc/keyword.h"

#include "qcc/unicode.h"

#include "qcc/preprocess.h"

#define NEXT(ptr) (*((ptr) + 1))
#define IS_NUMERIC(c) ('0' <= c && c <= '9')
#define IS_ALPHA(c) (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
#define IS_IDENT0(c) (IS_ALPHA(c) || c == '_')
#define IS_IDENT1(c) (IS_ALPHA(c) || c == '_' || IS_NUMERIC(c))


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
        else if (*source->cursor == '*') {
          if (NEXT(source->cursor) == '/') {
            source->cursor += 2;
            break;
          }
        }
        source->cursor++;
        if (!*source->cursor) {
          logger_fatal(-1, "Unterminated multi-line comment %s:%d\n",
            source->path, source->line);
        }
      }
      return delta;
  }
  return -1;
}

static int lexer_skip(Source* source) {
  int delta = 0;
  handle_ignore(source);
  while (*source->cursor == '/') {
    source->cursor++;
    delta = lexer_comment(source);
    if (delta == -1) {
      logger_fatal(-1, "Could not tokenize comment %s:%d\n",
        source->path, source->line);
    }
    source->line += delta;
    handle_ignore(source);
  }
  return delta;
}

static Token* lexer_opsep(Source* source) {
  Token tok;
  switch (*source->cursor) {
    case '+':
      switch (NEXT(source->cursor)) {
        case '+':
          tok = (Token){TOKEN_INC, 2, source->cursor, 0};
          break;
        case '=':
          tok = (Token){TOKEN_ASGN_ADD, 2, source->cursor, 0};
          break;
        default:
          tok = (Token){TOKEN_ADD, 1, source->cursor, 0};
          break;
      }
      break;
    case '-':
      switch (NEXT(source->cursor)) {
        case '-':
          tok = (Token){TOKEN_DEC, 2, source->cursor, 0};
          break;
        case '=':
          tok = (Token){TOKEN_ASGN_SUB, 2, source->cursor, 0};
          break;
        case '>':
          tok = (Token){TOKEN_ARROW, 2, source->cursor, 0};
          break;
        default:
          tok = (Token){TOKEN_SUB, 1, source->cursor, 0};
          break;
      }
      break;
    case '&':
      switch (NEXT(source->cursor)) {
        case '&':
          tok = (Token){TOKEN_LAND, 2, source->cursor, 0};
          break;
        case '=':
          tok = (Token){TOKEN_ASGN_BAND, 2, source->cursor, 0};
          break;
        default:
          tok = (Token){TOKEN_BAND, 1, source->cursor, 0};
          break;
      }
      break;
    case '|':
      switch (NEXT(source->cursor)) {
        case '|':
          tok = (Token){TOKEN_LOR, 2, source->cursor, 0};
          break;
        case '=':
          tok = (Token){TOKEN_ASGN_BOR, 2, source->cursor, 0};
          break;
        default:
          tok = (Token){TOKEN_BOR, 1, source->cursor, 0};
          break;
      }
      break;
    case '>':
      switch (NEXT(source->cursor)) {
        case '>':
          tok = (NEXT(source->cursor + 1) == '=')
            ? (Token){TOKEN_ASGN_BRSHIFT, 3, source->cursor, 0}
            : (Token){TOKEN_BRSHIFT, 2, source->cursor, 0};
          break;
        case '=':
          tok = (Token){TOKEN_GTE, 2, source->cursor, 0};
          break;
        default:
          tok = (Token){TOKEN_GT, 1, source->cursor, 0};
          break;
      }
      break;
    case '<':
      switch (NEXT(source->cursor)) {
        case '<':
          tok = (NEXT(source->cursor + 1) == '=')
            ? (Token){TOKEN_ASGN_BLSHIFT, 3, source->cursor, 0}
            : (Token){TOKEN_BLSHIFT, 2, source->cursor, 0};
          break;
        case '=':
          tok = (Token){TOKEN_LTE, 2, source->cursor, 0};
          break;
        default:
          tok = (Token){TOKEN_LT, 1, source->cursor, 0};
          break;
      }
      break;
    case '*':
      tok = (NEXT(source->cursor) == '=')
        ? (Token){TOKEN_ASGN_MUL, 2, source->cursor, 0}
        : (Token){TOKEN_MUL, 1, source->cursor, 0};
      break;
    case '/':
      tok = (NEXT(source->cursor) == '=')
        ? (Token){TOKEN_ASGN_DIV, 2, source->cursor, 0}
        : (Token){TOKEN_DIV, 1, source->cursor, 0};
      break;
    case '%':
      tok = (NEXT(source->cursor) == '=')
        ? (Token){TOKEN_ASGN_MOD, 2, source->cursor, 0}
        : (Token){TOKEN_MOD, 1, source->cursor, 0};
      break;
    case '!':
      tok = (NEXT(source->cursor) == '=')
        ? (Token){TOKEN_NE, 2, source->cursor, 0}
        : (Token){TOKEN_BNOT, 1, source->cursor, 0};
      break;
    case '^':
      tok = (NEXT(source->cursor) == '=')
        ? (Token){TOKEN_ASGN_BXOR, 2, source->cursor, 0}
        : (Token){TOKEN_BXOR, 1, source->cursor, 0};
      break;
    case '=':
      tok = (NEXT(source->cursor) == '=')
        ? (Token){TOKEN_EQ, 2, source->cursor, 0}
        : (Token){TOKEN_ASSIGN, 1, source->cursor, 0};
      break;
    case '~': tok = (Token){TOKEN_BNOT, 1, source->cursor, 0}; break;
    case ';': tok = (Token){TOKEN_SEMICOLON, 1, source->cursor, 0}; break;
    case ':': tok = (Token){TOKEN_COLON, 1, source->cursor, 0}; break;
    case ',': tok = (Token){TOKEN_COMMA, 1, source->cursor, 0}; break;
    case '(': tok = (Token){TOKEN_LPAREN, 1, source->cursor, 0}; break;
    case ')': tok = (Token){TOKEN_RPAREN, 1, source->cursor, 0}; break;
    case '[': tok = (Token){TOKEN_LBRACKET, 1, source->cursor, 0}; break;
    case ']': tok = (Token){TOKEN_RBRACKET, 1, source->cursor, 0}; break;
    case '{': tok = (Token){TOKEN_LBRACE, 1, source->cursor, 0}; break;
    case '}': tok = (Token){TOKEN_RBRACE, 1, source->cursor, 0}; break;
    case '.': tok = (Token){TOKEN_DOT, 1, source->cursor, 0}; break;
    case '?': tok = (Token){TOKEN_QUESTION, 1, source->cursor, 0}; break;
    default: return 0;
  }
  source->cursor += tok.length;
  return token_create(tok.kind, tok.loc, tok.length, tok.value);
}

static Token* lexer_identifier(Source* source) {
  char* origin = source->cursor;
  while (IS_IDENT1(*source->cursor))
    source->cursor++;

  return token_create(TOKEN_IDENTIFIER,
      origin,
      (++source->cursor-origin) - 1,
      0
    );
}

static Token* lexer_charconst(Source* source, encoding_t encoding) {}
static Token* lexer_strconst(Source* source, encoding_t encoding) {}

static Token* lexer_hexintconst(Source* source) {}
static Token* lexer_octintconst(Source* source) {}

static Token* lexer_numconst(Source* source) {
  // base prefix
  int base = 10;
  if (*source->cursor == '0') {
    // hexadecimal [0-9a-f]
    if (NEXT(source->cursor) == 'x' || NEXT(source->cursor) == 'X')
      return lexer_hexintconst(source);
    // octal [0-7]
    if (IS_NUMERIC(NEXT(source->cursor)))
      return lexer_octintconst(source);
  }
}

static Token* lexer_internal(Lexer* lexer) {
  Source* source = (Source *)lexer->sources->value;
  if (*source->cursor) {
    lexer_skip(source);

    // handle preprocessor directive here
    while (*source->cursor == '#') {
      preprocess_directive(lexer);
      source = (Source *)lexer->sources->value;
      lexer_skip(source);
    }

    // TODO: tokenize str/char const & wide unicode

    // tokenize numeric constants
    if (IS_NUMERIC(*source->cursor))
      return lexer_numconst(source);

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
        tok->kind = TOKEN_KEYWORD;
        tok->value = (Keyword *)kwrd_entry->value;
      }
      return tok;
    }

    // tokenize operators and separators
    Token* tok = lexer_opsep(source);
    if (tok)
      return tok;
  }
  return token_create(TOKEN_EOF, source->cursor, 0, 0);
}

/* --- public lexer api --- */
Lexer* lexer_create(void) {
  Lexer* lexer = malloc(sizeof(Lexer));
  lexer->sources = NULL;
  lexer->keywords = hashmap_create();
  for (int i = 0; i < KWRD_END; ++i) {
    hashmap_insert(lexer->keywords, &(Entry) {
      .key = kwrd_map[i]->str,
      .value = (void *)kwrd_map[i]
    });
  }
  // TODO: implement incremental preprocessor
  lexer->macros = hashmap_create();
  // load pre defined macro map
  lexer->expand = NULL;
  return lexer;
}

void lexer_destroy(Lexer* lexer) {
  if (lexer) {
    // lexer->sources passed as pointer, dont dealloc
    hashmap_destroy(lexer->keywords);
    hashmap_destroy(lexer->macros);
    list_destroy(lexer->expand);
    free(lexer);
  }
  return;
}

int lexer_register(Lexer* lexer, const char* path) {
  if (lexer) {
    Source* source = source_create(path);
    if (source)
      return list_fpush(&lexer->sources, source);
  }
  return -1;
}

Token* lexer_get(Lexer* lexer) {
  if (lexer && list_length(lexer->sources) > 0) {
    Source* source = (Source *)lexer->sources->value;
    if (!source->is_ready)
      source_fill(source);

    Token* tok = lexer_internal(lexer);
    if (tok->kind == TOKEN_EOF) {
      source_destroy(
        list_fpop(&lexer->sources)
      );
    }
    return tok;
  }
  return 0;
}

Token* lexer_peek(Lexer* lexer) {
  Token* tok;
  if (tok = lexer_get(lexer)) {
    Source* source = (Source *)lexer->sources->value;
    source->cursor -= tok->length;
    return tok;
  }
  return 0;
}

int lexer_eat(Lexer* lexer) {
  Token* tok;
  if (tok = lexer_get(lexer)) {
    int length = tok->length;
    free(tok);
    return length;
  }
  return 0;
}

