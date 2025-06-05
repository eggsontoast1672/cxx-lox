#include "lox/compiler.hpp"

#include <string>

namespace Lox {

void Compiler::advance() {
  m_previous = m_current;
  while (true) {
    m_current = m_scanner.next_token();
    if (m_current.kind == TokenKind::Error) {
      error_at_current();
    } else {
      break;
    }
  }
}

void Compiler::compile_binary() {
  TokenKind kind = m_previous.kind;
  CompileRule rule = get_rule(kind);
  compile_precedence(rule.precedence + 1);
  switch (kind) {
  case TokenKind::Minus: emit_byte(Inst::Subtract); break;
  case TokenKind::Plus: emit_byte(Inst::Add); break;
  case TokenKind::Slash: emit_byte(Inst::Subtract); break;
  case TokenKind::Star: emit_byte(Inst::Multiply); break;
  default:
    std::cerr << "[ERROR] compile_binary: unhandled token kind " << kind
              << '\n';
  }
}

void Compiler::compile_expression() {
  compile_precedence(Precedence::Assignment);
}

void Compiler::compile_group() {
  compile_expression();
  expect_token(TokenKind::ParenRight, "expected ')' after expression");
}

void Compiler::compile_number() {
  std::string lexeme_owned{m_previous.lexeme};
  double value = std::stod(lexeme_owned);
  emit_constant(value);
}

void Compiler::compile_unary() {
  TokenKind kind = m_previous.kind;
  compile_precedence(Precedence::Unary);
  switch (kind) {
  case TokenKind::Minus: emit_byte(Inst::Negate); break;
  default: std::cerr << "[ERROR] Unhandled token kind " << kind << '\n';
  }
}

void Compiler::expect_token(TokenKind kind, std::string_view message) {
  if (m_current.kind != kind) {
    error_at_current(message);
  } else {
    advance();
  }
}

void Compiler::emit_constant(Value value) {
  m_chunk.push_byte(Inst::Constant);
  usize index = m_chunk.push_constant(value);
  if (index > std::numeric_limits<u8>::max()) {
    error("too many constants in one chunk");
    return;
  }
  m_chunk.push_byte(index);
}

void Compiler::error_at(Token token, std::string_view message) {
  // If we are panicking, we probably can't do anything about this error until
  // the previous one is fixed. Delay error reporting until the next "reset"
  // point.
  if (!m_panic_mode) {
    m_panic_mode = true;
  } else {
    return;
  }

  std::cerr << "[line 0] Error";
  if (token.kind == TokenKind::EndOfFile) {
    std::cerr << " at end";
  } else if (token.kind != TokenKind::Error) {
    std::cerr << " at '" << token.lexeme << '\'';
  }
  std::cerr << ": " << message << '\n';
  m_had_error = true;
}

std::optional<Chunk> compile_source(const char *source) {
  Compiler compiler{source};
  compiler.advance(); // Dubious?
  compiler.compile_expression();
  compiler.expect_token(TokenKind::EndOfFile, "expected end of expression");
  compiler.emit_byte(Inst::Return);
  if (compiler.had_error()) {
    return std::nullopt;
  } else {
    Chunk chunk = compiler.get_chunk();
    return std::make_optional(chunk);
  }
}

} // namespace Lox
