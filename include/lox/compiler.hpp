#pragma once

#include "lox/chunk.hpp"
#include "lox/scanner.hpp"

namespace Lox {

enum class Precedence {
  None,
  Assignment,
  Or,
  And,
  Equality,
  Comparison,
  Term,
  Factor,
  Unary,
  Call,
  Primary,
};

class Compiler {
public:
  inline Compiler(const char *source) : m_scanner{source} {}

  void advance();
  void expect_token(TokenKind kind, std::string_view message);
  inline Chunk get_chunk() const { return m_chunk; } // TODO why return copy?
  inline bool had_error() const { return m_had_error; }

  void compile_binary();
  void compile_expression();
  void compile_group();
  void compile_number();
  void compile_unary();

  inline void emit_byte(u8 byte) { m_chunk.push_byte(byte); }
  void emit_constant(Value value);

  void error_at(Token token, std::string_view message);
  void error_at_current(std::string_view message);

private:
  Scanner m_scanner;
  Token m_current;
  Token m_previous;

  Chunk m_chunk;
  bool m_had_error = false;
  bool m_panic_mode = false;
};

std::optional<Chunk> compile_source(const char *source);

} // namespace Lox
