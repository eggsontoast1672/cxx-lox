#pragma once

#include <iostream>
#include <vector>

#include "lox/types.hpp"
#include "lox/value.hpp"

namespace Lox {

enum Inst : u8 {
  Constant,
  Add,
  Subtract,
  Multiply,
  Divide,
  Negate,
  Return,
};

std::ostream &operator<<(std::ostream &stream, Inst inst);

class Chunk {
public:
  inline usize count() const { return m_instructions.size(); }

  /// Retrieves the byte at the supplied offset.
  ///
  /// Note that the byte which was retrieved might not be a valid instruction,
  /// since it may be the operand of a previous instruction. Use with care.
  ///
  /// If the chunk contains no byte at the supplied index, then
  /// std::out_of_range is thrown.
  inline u8 get_byte(usize offset) const { return m_instructions.at(offset); }

  /// Retrieves the constant at the supplied index.
  ///
  /// If the constant pool of the chunk has no value at the supplied index, then
  /// std::out_of_range is thrown.
  inline Value get_constant(usize index) const { return m_constants.at(index); }

  /// Pushes a byte to the chunk.
  ///
  /// Note that this method does not verify the semantic correctness of the
  /// bytes being pushed, so use it with care.
  inline void push_byte(u8 byte) { m_instructions.push_back(byte); }

  /// Pushes a value to the constant pool.
  ///
  /// Returns the index in the pool where the value was pushed. Though this
  /// method returns a usize, it is guaranteed that the return value can be
  /// safely cast to a u8.
  ///
  /// If the caller attempts to insert more than std::numeric_limits<u8>::max()
  /// constants into the pool, then std::length_error is thrown.
  usize push_constant(Value value);

private:
  std::vector<u8> m_instructions;
  std::vector<Value> m_constants;
};

} // namespace Lox
