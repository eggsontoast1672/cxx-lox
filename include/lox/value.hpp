#pragma once

#include <vector>

#include "lox/types.hpp"

namespace Lox {

using Value = double;

class ValueArray {
public:
  inline usize count() const { return m_values.size(); }
  inline void push(Value value) { m_values.push_back(value); }

private:
  std::vector<Value> m_values;
};

}; // namespace Lox
