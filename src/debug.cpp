#include "lox/debug.hpp"

#include <iomanip>
#include <iostream>

namespace Lox::Debug {

void print_chunk(const Chunk &chunk, const char *name) {
  std::cout << '<' << name << ">:\n";
  for (usize offset = 0; offset < chunk.count();) {
    offset = print_instruction(chunk, offset);
  }
}

usize print_instruction(const Chunk &chunk, usize offset) {
  // Instruction "address"
  std::cout << "  " << std::hex << std::setfill('0') << std::setw(6) << offset
            << ":\t";

  u8 byte = chunk.get_byte(offset);
  switch (byte) {
  case Inst::Constant: {
    u8 constant = chunk.get_byte(offset + 1);
    std::cout << std::setw(2) << static_cast<int>(byte) << ' ' << std::setw(2)
              << static_cast<int>(constant) << '\t' << static_cast<Inst>(byte)
              << '\t' << chunk.get_constant(constant) << '\n';
    return offset + 2;
  }
  case Inst::Add:
  case Inst::Subtract:
  case Inst::Multiply:
  case Inst::Divide:
  case Inst::Negate:
  case Inst::Return:
    std::cout << std::setw(2) << static_cast<int>(byte) << '\t'
              << static_cast<Inst>(byte) << '\n';
    return offset + 1;
  default:
    std::cout << "Unknown opcode " << byte << '\n';
    return offset + 1;
  }
}

void print_stack(const std::vector<Value> &stack) {
  for (const Value &value : stack) {
    std::cout << "[ " << value << " ]";
  }
  std::cout << '\n';
}

} // namespace Lox::Debug
