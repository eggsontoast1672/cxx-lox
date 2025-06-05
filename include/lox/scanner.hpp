#pragma once

#include <string_view>

#include "lox/types.hpp"

namespace Lox {

enum class TokenKind {
  BraceLeft,
  BraceRight,
  Comma,
  Dot,
  Minus,
  ParenLeft,
  ParenRight,
  Plus,
  Semicolon,
  Slash,
  Star,
  Bang,
  BangEqual,
  Equal,
  EqualEqual,
  Greater,
  GreaterEqual,
  Less,
  LessEqual,
  Identifier,
  Number,
  String,
  And,
  Class,
  Else,
  False,
  For,
  Fun,
  If,
  Nil,
  Or,
  Print,
  Return,
  Super,
  This,
  True,
  Var,
  While,
  Error,
  EndOfFile,
};

std::ostream &operator<<(std::ostream &stream, TokenKind kind);

struct Token {
  TokenKind kind;
  std::string_view lexeme;
};

std::ostream &operator<<(std::ostream &stream, Token token);

class Scanner {
public:
  inline Scanner(const char *source) : m_source{source} {}

  /// Advances the scanner.
  ///
  /// The next character in the stream is returned and the scanner state is
  /// advanced by one. If the stream has no more characters, a null byte is
  /// returned. However, if a previous call to is_at_end returns false, then
  /// this method is guaranteed to return a valid character from the source.
  char advance();

  /// Returns true if the scanner is out of characters and false otherwise.
  ///
  /// If this method returns false, then advance can be called without
  /// exception.
  inline bool has_tokens() const { return m_current < m_source.size(); }

  /// Gets the next token in the stream.
  ///
  /// If there are no tokens remaining, then a token with kind
  /// TokenKind::EndOfFile is returned.
  Token next_token();

private:
  std::string_view m_source;
  usize m_start = 0;
  usize m_current = 0;

  TokenKind check_keyword(int depth, std::string_view rest,
                          TokenKind kind) const;
  TokenKind get_ident_kind() const;

  /// Is the given character alpha?
  ///
  /// This is used to determine which characters are part of an identifier. All
  /// letters from A-Z and a-z are considered alpha, in addition to the
  /// underscore character.
  static inline bool is_alpha(char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_';
  }

  /// Is the given character a digit?
  ///
  /// Returns true if the character passed is any of ascii '0' through ascii
  /// '9', and false otherwise.
  static inline bool is_digit(char c) { return '0' <= c && c <= '9'; }

  static Token make_error_token(const char *message);

  Token make_identifier();
  Token make_number();
  Token make_string();
  Token make_token(TokenKind kind) const;

  /// Matches against the next character in the stream.
  ///
  /// If the next character matches the expected one, then the character is
  /// consumed and true is returned. If the scanner has no more characters or
  /// the character did not match the expected one, the scanner state is not
  /// altered and false is returned.
  bool match(char expected);

  /// Peeks at the next character.
  ///
  /// Returns the next character in the stream without consuming it. If the
  /// stream has no more characters, a null byte is returned.
  char peek() const;

  /// Peeks at the next-after character.
  ///
  /// This method looks past the next character and returns the one after that
  /// while preserving the scanner's state. If no such character exists, a null
  /// byte is returned.
  char peek_next() const;

  /// Consumes whitespace characters.
  ///
  /// This method consumes as many whitespace characters in the stream as
  /// possible. The characters considered as whitespace are spaces, carriage
  /// returns, and horizontal tab characters. Note that no unicode whitespace
  /// characters are handled, only ascii.
  ///
  /// In addition to whitespace characters, all characters after a double
  /// forward slash up to the next newline are considered whitespace, since
  /// comments are to be ignored by the compiler.
  void skip_whitespace();
};

} // namespace Lox
