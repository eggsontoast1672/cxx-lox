#include "lox/scanner.hpp"

#include <iostream>
#include <unordered_map>

namespace Lox {

static std::unordered_map<TokenKind, const char *> s_token_names = {
    {TokenKind::BraceLeft, "TokenKind::BraceLeft"},
    {TokenKind::BraceRight, "TokenKind::BraceRight"},
    {TokenKind::Comma, "TokenKind::Comma"},
    {TokenKind::Dot, "TokenKind::Dot"},
    {TokenKind::Minus, "TokenKind::Minus"},
    {TokenKind::ParenLeft, "TokenKind::ParenLeft"},
    {TokenKind::ParenRight, "TokenKind::ParenRight"},
    {TokenKind::Plus, "TokenKind::Plus"},
    {TokenKind::Semicolon, "TokenKind::Semicolon"},
    {TokenKind::Slash, "TokenKind::Slash"},
    {TokenKind::Star, "TokenKind::Star"},
    {TokenKind::Bang, "TokenKind::Bang"},
    {TokenKind::BangEqual, "TokenKind::BangEqual"},
    {TokenKind::Equal, "TokenKind::Equal"},
    {TokenKind::EqualEqual, "TokenKind::EqualEqual"},
    {TokenKind::Greater, "TokenKind::Greater"},
    {TokenKind::GreaterEqual, "TokenKind::GreaterEqual"},
    {TokenKind::Less, "TokenKind::Less"},
    {TokenKind::LessEqual, "TokenKind::LessEqual"},
    {TokenKind::Identifier, "TokenKind::Identifier"},
    {TokenKind::Number, "TokenKind::Number"},
    {TokenKind::String, "TokenKind::String"},
    {TokenKind::And, "TokenKind::And"},
    {TokenKind::Class, "TokenKind::Class"},
    {TokenKind::Else, "TokenKind::Else"},
    {TokenKind::False, "TokenKind::False"},
    {TokenKind::For, "TokenKind::For"},
    {TokenKind::Fun, "TokenKind::Fun"},
    {TokenKind::If, "TokenKind::If"},
    {TokenKind::Nil, "TokenKind::Nil"},
    {TokenKind::Or, "TokenKind::Or"},
    {TokenKind::Print, "TokenKind::Print"},
    {TokenKind::Return, "TokenKind::Return"},
    {TokenKind::Super, "TokenKind::Super"},
    {TokenKind::This, "TokenKind::This"},
    {TokenKind::True, "TokenKind::True"},
    {TokenKind::Var, "TokenKind::Var"},
    {TokenKind::While, "TokenKind::While"},
    {TokenKind::Error, "TokenKind::Error"},
    {TokenKind::EndOfFile, "TokenKind::EndOfFile"},
};

std::ostream &operator<<(std::ostream &stream, TokenKind kind) {
  auto it = s_token_names.find(kind);
  if (it != s_token_names.end()) {
    stream << it->second;
  } else {
    std::cerr << "[ERROR] Attempted to access name of token "
              << static_cast<int>(kind) << '\n';
  }
  return stream;
}

std::ostream &operator<<(std::ostream &stream, Token token) {
  stream << "{ " << token.kind << ", \"" << token.lexeme << "\" }";
  return stream;
}

char Scanner::advance() {
  if (m_current < m_source.size()) {
    return m_source[m_current++];
  } else {
    return '\0';
  }
}

Token Scanner::next_token() {
  skip_whitespace();
  m_start = m_current;
  char c = advance();
  switch (c) {
  case '{': return make_token(TokenKind::BraceLeft);
  case '}': return make_token(TokenKind::BraceRight);
  case ',': return make_token(TokenKind::Comma);
  case '.': return make_token(TokenKind::Dot);
  case '-': return make_token(TokenKind::Minus);
  case '(': return make_token(TokenKind::ParenLeft);
  case ')': return make_token(TokenKind::ParenRight);
  case '+': return make_token(TokenKind::Plus);
  case ';': return make_token(TokenKind::Semicolon);
  case '/': return make_token(TokenKind::Slash);
  case '*': return make_token(TokenKind::Star);
  case '\0': return make_token(TokenKind::EndOfFile);
  case '!':
    if (match('=')) {
      return make_token(TokenKind::BangEqual);
    } else {
      return make_token(TokenKind::Bang);
    }
  case '=':
    if (match('=')) {
      return make_token(TokenKind::EqualEqual);
    } else {
      return make_token(TokenKind::Equal);
    }
  case '>':
    if (match('=')) {
      return make_token(TokenKind::GreaterEqual);
    } else {
      return make_token(TokenKind::Greater);
    }
  case '<':
    if (match('=')) {
      return make_token(TokenKind::LessEqual);
    } else {
      return make_token(TokenKind::Less);
    }
  default:
    if (is_alpha(c)) {
      return make_identifier();
    } else if (is_digit(c)) {
      return make_number();
    } else {
      return make_error_token("Unexpected character.");
    }
  }
}

TokenKind Scanner::check_keyword(int start, std::string_view rest,
                                 TokenKind kind) const {
  bool length_matches = m_current - m_start == start + rest.size();
  bool content_matches = m_source.substr(m_start, m_current - m_start) == rest;
  if (length_matches && content_matches) {
    return kind;
  }
  return TokenKind::Identifier;
}

TokenKind Scanner::get_ident_kind() const {
  switch (m_source[m_start]) {
  case 'a': return check_keyword(1, "nd", TokenKind::And);
  case 'c': return check_keyword(1, "lass", TokenKind::Class);
  case 'e': return check_keyword(1, "lse", TokenKind::Else);
  case 'i': return check_keyword(1, "f", TokenKind::If);
  case 'n': return check_keyword(1, "il", TokenKind::Nil);
  case 'o': return check_keyword(1, "r", TokenKind::Or);
  case 'p': return check_keyword(1, "rint", TokenKind::Print);
  case 'r': return check_keyword(1, "eturn", TokenKind::Return);
  case 's': return check_keyword(1, "uper", TokenKind::Super);
  case 'v': return check_keyword(1, "ar", TokenKind::Var);
  case 'w': return check_keyword(1, "hile", TokenKind::While);
  }
  return TokenKind::Identifier;
}

Token Scanner::make_error_token(const char *message) {
  return Token{
      .kind = TokenKind::Error,
      .lexeme = message,
  };
}

Token Scanner::make_identifier() {
  while (is_alpha(peek()) || is_digit(peek())) {
    advance();
  }
  TokenKind kind = get_ident_kind();
  return make_token(kind);
}

Token Scanner::make_number() {
  while (is_digit(peek())) {
    advance();
  }
  if (peek() == '.' && is_digit(peek_next())) {
    advance();
    while (is_digit(peek())) {
      advance();
    }
  }
  return make_token(TokenKind::Number);
}

Token Scanner::make_string() {
  while (has_tokens() && peek() != '"') {
    advance();
  }
  if (has_tokens()) {
    advance();
    return make_token(TokenKind::String);
  } else {
    return make_error_token("Unterminated string.");
  }
}

Token Scanner::make_token(TokenKind kind) const {
  return Token{
      .kind = kind,
      .lexeme = m_source.substr(m_start, m_current - m_start),
  };
}

bool Scanner::match(char expected) {
  if (!has_tokens() || m_source[m_current] != expected) {
    return false;
  }
  m_current++;
  return true;
}

char Scanner::peek() const {
  if (m_current >= m_source.size()) {
    return '\0';
  }
  return m_source[m_current];
}

char Scanner::peek_next() const {
  if (m_current + 1 >= m_source.size()) {
    return '\0';
  }
  return m_source[m_current + 1];
}

void Scanner::skip_whitespace() {
  while (true) {
    char c = peek();
    if (c == ' ' || c == '\r' || c == '\n') {
      advance();
    } else if (c == '/') {
      if (peek_next() == '/') {
        while (has_tokens() && peek() != '\n') {
          advance();
        }
      }
    } else {
      break;
    }
  }
}

} // namespace Lox
