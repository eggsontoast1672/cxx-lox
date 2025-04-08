#include "lox/debug.hpp"

#include <iomanip>
#include <iostream>

class Disassembler {
public:
  Disassembler(const Lox::Chunk &chunk) : m_chunk{chunk} {}

  void print(const char *name) {
    std::cout << "== " << name << " ==\n";
    while (m_offset < m_chunk.count()) {
      print_next_instruction();
    }
  }

private:
  const Lox::Chunk &m_chunk;
  usize m_offset = 0;

  void print_next_instruction() {
    std::cout << std::setw(4) << std::setfill('0') << m_offset << ' ';
    u8 byte = m_chunk.get(m_offset);
    switch (byte) {
    case Lox::Inst::Return:
      print_simple_instruction("OpCode::Return");
      break;
    default:
      std::cout << "Unknown opcode " << byte << '\n';
      m_offset++;
    }
  }

  void print_simple_instruction(const char *name) {
    std::cout << name << '\n';
    m_offset++;
  }
};

namespace Lox {

void disassemble(const Chunk &chunk, const char *name) {
  Disassembler disassembler{chunk};
  disassembler.print(name);
}

} // namespace Lox
