#include "lox/chunk.hpp"

namespace Lox {

std::ostream &operator<<(std::ostream &stream, Inst code) {
  stream << static_cast<u8>(code);
  return stream;
}

usize Chunk::push_constant(Value value) {
  m_constants.push(value);
  return m_constants.count() - 1;
}

}; // namespace Lox
