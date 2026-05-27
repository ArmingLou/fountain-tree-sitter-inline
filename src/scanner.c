#include "tree_sitter/parser.h"
#include <string.h>
#include <stdlib.h>

enum TokenType {
  INLINE_NOTE,
  INLINE_BONEYARD,
};

void *tree_sitter_fountain_inline_external_scanner_create() {
  return NULL;
}

bool tree_sitter_fountain_inline_external_scanner_scan(
  void *payload, TSLexer *lexer, const bool *valid_symbols
) {
  // Try inline note ([[...]]) - 支持跨行
  if (valid_symbols[INLINE_NOTE] && lexer->lookahead == '[') {
    lexer->advance(lexer, false);
    if (lexer->lookahead == '[') {
      lexer->advance(lexer, false);
      int depth = 1;
      while (lexer->lookahead != '\0') {
        if (lexer->lookahead == '[') {
          lexer->advance(lexer, false);
          if (lexer->lookahead == '[') depth++;
          continue;
        }
        if (lexer->lookahead == ']') {
          lexer->advance(lexer, false);
          if (lexer->lookahead == ']') {
            lexer->advance(lexer, false);
            depth--;
            if (depth == 0) {
              lexer->result_symbol = INLINE_NOTE;
              lexer->mark_end(lexer);
              return true;
            }
            continue;
          }
          continue;
        }
        lexer->advance(lexer, false);
      }
    }
    return false;
  }

  // Try inline boneyard (/*...*/) - 支持跨行
  if (valid_symbols[INLINE_BONEYARD] && lexer->lookahead == '/') {
    lexer->advance(lexer, false);
    if (lexer->lookahead == '*') {
      lexer->advance(lexer, false);
      while (lexer->lookahead != '\0') {
        if (lexer->lookahead == '*') {
          lexer->advance(lexer, false);
          if (lexer->lookahead == '/') {
            lexer->advance(lexer, false);
            lexer->result_symbol = INLINE_BONEYARD;
            lexer->mark_end(lexer);
            return true;
          }
          continue;
        }
        lexer->advance(lexer, false);
      }
    }
    return false;
  }

  return false;
}

void *tree_sitter_fountain_inline_external_scanner_destroy(void *payload) {
  return NULL;
}

unsigned tree_sitter_fountain_inline_external_scanner_serialize(
  void *payload, char *buffer
) {
  return 0;
}

void tree_sitter_fountain_inline_external_scanner_deserialize(
  void *payload, const char *buffer, unsigned length
) {
}
