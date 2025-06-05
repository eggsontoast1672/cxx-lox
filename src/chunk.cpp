#include "lox/chunk.hpp"

#include <limits>

namespace Lox {

std::ostream &operator<<(std::ostream &stream, Inst code) {
  switch (code) {
  case Inst::Constant: stream << "Inst::Constant"; break;
  case Inst::Add: stream << "Inst::Add"; break;
  case Inst::Subtract: stream << "Inst::Subtract"; break;
  case Inst::Multiply: stream << "Inst::Multiply"; break;
  case Inst::Divide: stream << "Inst::Divide"; break;
  case Inst::Negate: stream << "Inst::Negate"; break;
  case Inst::Return: stream << "Inst::Return"; break;
  }
  return stream;
}

usize Chunk::push_constant(Value value) {
  usize count = m_constants.size();
  m_constants.push_back(value);
  return count;
}

}; // namespace Lox
