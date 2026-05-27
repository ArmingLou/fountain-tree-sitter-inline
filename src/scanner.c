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
  // 先跳过前导空格（支持缩进的情况）
  while (lexer->lookahead == ' ' || lexer->lookahead == '\t') {
    lexer->advance(lexer, false);
  }
  
  // 如果跳过空格后到达换行符，返回 false 让 text 处理
  if (lexer->lookahead == '\n') {
    return false;
  }

  // Try inline note ([[...]]) - 直接匹配，不检查 valid_symbols
  // 支持跨行匹配
  if (lexer->lookahead == '[') {
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
        // 支持跨行：遇到换行符继续匹配
        if (lexer->lookahead == '\n') {
          lexer->advance(lexer, false);
          // 跳过换行后的空格
          while (lexer->lookahead == ' ' || lexer->lookahead == '\t') {
            lexer->advance(lexer, false);
          }
          continue;
        }
        lexer->advance(lexer, false);
      }
    }
  }

  // Try inline boneyard (/*...*/) - 直接匹配，不检查 valid_symbols
  // 支持跨行匹配
  if (lexer->lookahead == '/') {
    lexer->advance(lexer, false);
    if (lexer->lookahead == '*') {
      lexer->advance(lexer, false);
      while (lexer->lookahead != '\0') {
        // 支持跨行：遇到换行符继续匹配
        if (lexer->lookahead == '\n') {
          lexer->advance(lexer, false);
          // 跳过换行后的空格
          while (lexer->lookahead == ' ' || lexer->lookahead == '\t') {
            lexer->advance(lexer, false);
          }
          continue;
        }
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