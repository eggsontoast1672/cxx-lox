#include "lox/vm.hpp"

#include "lox/debug.hpp"

namespace Lox {

InterpretResult VirtualMachine::interpret(const Chunk &chunk) {
  m_chunk = &chunk;
  m_ip = 0;
  return run();
}

Value VirtualMachine::pop_back() {
  Value value = m_stack.back();
  m_stack.pop_back();
  return value;
}

InterpretResult VirtualMachine::run() {
  while (true) {
#ifndef NDEBUG
    Debug::print_instruction(*m_chunk, m_ip);
    // Debug::print_stack(m_stack);
#endif

    u8 byte = read_byte();
    switch (byte) {
    case Inst::Constant: {
      Value constant = read_constant();
      m_stack.push_back(constant);
      break;
    }
    case Inst::Add:
      run_binop<std::plus<double>>();
      break;
    case Inst::Subtract:
      run_binop<std::minus<double>>();
      break;
    case Inst::Multiply:
      run_binop<std::multiplies<double>>();
      break;
    case Inst::Divide:
      run_binop<std::divides<double>>();
      break;
    case Inst::Negate:
      m_stack.push_back(-pop_back());
      break;
    case Inst::Return:
      std::cout << pop_back() << '\n';
      return InterpretResult::Ok;
    }
  }
}

} // namespace Lox
