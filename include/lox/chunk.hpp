#pragma once

#include <iostream>
#include <vector>

#include "lox/types.hpp"
#include "lox/value.hpp"

namespace Lox {

enum Inst : u8 {
  Constant,
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
  /// Throws out_of_range if the chunk contains no instruction at the supplied
  /// offset.
  inline u8 get(usize offset) const { return m_instructions.at(offset); }

  /// Pushes a value to the constant pool.
  ///
  /// Returns the index in the pool where the value was pushed. Though this
  /// method returns a usize, it is guaranteed that the return value can be
  /// safely cast to a u8.
  usize push_constant(Value value);

  /// Pushes a byte to the chunk.
  ///
  /// Note that this method does not verify the semantic correctness of the
  /// bytes being pushed, so use it with care.
  inline void push_byte(u8 byte) { m_instructions.push_back(byte); }

private:
  std::vector<u8> m_instructions;
  ValueArray m_constants;
};

} // namespace Lox
