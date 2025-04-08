#pragma once

#include "lox/chunk.hpp"

namespace Lox {

void disassemble(const Chunk &chunk, const char *name);

} // namespace Lox
