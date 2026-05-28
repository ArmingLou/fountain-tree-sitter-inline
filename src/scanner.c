#include "tree_sitter/parser.h"
#include <string.h>
#include <stdlib.h>

enum {
  sym_paren_text = 0,
  sym_boneyard_start = 1,
  sym_note_start = 2,
};

void *tree_sitter_fountain_inline_external_scanner_create() {
  return NULL;
}

bool tree_sitter_fountain_inline_external_scanner_scan(
  void *payload, TSLexer *lexer, const bool *valid_symbols
) {
  // ==== 1. NOTE_START [[ ====
  if (valid_symbols[sym_note_start] && lexer->lookahead == '[') {
    lexer->advance(lexer, false);
    if (lexer->lookahead == '[') {
      lexer->advance(lexer, false);
      lexer->result_symbol = sym_note_start;
      lexer->mark_end(lexer);
      return true;
    }
    lexer->lookahead = '[';
    return false;
  }

  // ==== 2. BONEYARD_START /* ====
  if (valid_symbols[sym_boneyard_start] && lexer->lookahead == '/') {
    lexer->advance(lexer, false);
    if (lexer->lookahead == '*') {
      lexer->advance(lexer, false);
      lexer->result_symbol = sym_boneyard_start;
      lexer->mark_end(lexer);
      return true;
    }
    lexer->lookahead = '/';
    return false;
  }

  // ==== 3. PAREN_TEXT - 行首括号（col==0），支持跨行 ====
  if (valid_symbols[sym_paren_text]) {
    if (lexer->lookahead == '(' || lexer->lookahead == 0xFF08) {
      // 只在行首匹配
      if (lexer->get_column(lexer) != 0) return false;
      
      int32_t close_paren = (lexer->lookahead == '(') ? ')' : 0xFF09;
      lexer->advance(lexer, false);
      
      bool found_close = false;
      while (lexer->lookahead != '\0') {
        if (lexer->lookahead == close_paren) {
          lexer->advance(lexer, false);
          if (lexer->lookahead == '\n' || lexer->lookahead == '\0') {
            found_close = true;
            break;
          }
          continue;
        }
        if (lexer->lookahead == '\n') {
          lexer->advance(lexer, false);
          while (lexer->lookahead == ' ' || lexer->lookahead == '\t') {
            lexer->advance(lexer, false);
          }
          continue;
        }
        lexer->advance(lexer, false);
      }
      
      if (found_close) {
        lexer->result_symbol = sym_paren_text;
        lexer->mark_end(lexer);
        return true;
      }
    }
  }

  return false;
}

void *tree_sitter_fountain_inline_external_scanner_destroy(void *payload) {
  return NULL;
}

unsigned tree_sitter_fountain_inline_external_scanner_serialize(void *payload, char *buffer) {
  return 0;
}

void tree_sitter_fountain_inline_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
}