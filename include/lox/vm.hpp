#pragma once

#include <vector>

#include "lox/chunk.hpp"

namespace Lox {

enum class InterpretResult {
  Ok,
  CompileError,
  RuntimeError,
};

class VirtualMachine {
public:
  void compile_source(const std::string &source);

  /// Executes a bytecode chunk.
  InterpretResult interpret_chunk(const Chunk &chunk);

  /// Executes a source code string.
  InterpretResult interpret_source(const std::string &code);

private:
  const Chunk *m_chunk = nullptr;
  usize m_ip = 0;

  // We use std::vector here instead of the more obvious std::stack because we
  // need to be able to iterate over it, and std::stack does not expose
  // iterators.
  std::vector<Value> m_stack;

  Value pop_back();
  inline u8 read_byte() { return m_chunk->get_byte(m_ip++); }
  inline Value read_constant() { return m_chunk->get_constant(read_byte()); }
  InterpretResult run();

  template <class Operator> void run_binop() {
    double right = pop_back();
    double left = pop_back();
    m_stack.push_back(Operator{}(left, right));
  }
};

} // namespace Lox
