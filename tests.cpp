#include <catch2/catch_test_macros.hpp>

#include "include/lox/scanner.hpp"

using namespace Lox;

TEST_CASE("Single characters are scanned properly", "[what_goes_here]") {
  const char *source = "{},.-()+;/*";
  Scanner scanner{source};
  REQUIRE(scanner.next_token() == Token{TokenKind::BraceLeft, "{"});
}
