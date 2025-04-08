#include "lox/chunk.hpp"
#include "lox/debug.hpp"

int main(int argc, const char **argv) {
  Lox::Chunk chunk;

  u8 constant = chunk.push_constant(1.2);
  chunk.push_byte(Lox::Inst::Constant);
  chunk.push_byte(constant);

  Lox::disassemble(chunk, "test chunk");
}
