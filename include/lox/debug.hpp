#pragma once

#include "lox/chunk.hpp"

namespace Lox::Debug {

void print_chunk(const Chunk &chunk, const char *name);
usize print_instruction(const Chunk &chunk, usize offset);
void print_stack(const std::vector<Value> &stack);

} // namespace Lox::Debug
