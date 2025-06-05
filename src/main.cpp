#include <fstream>
#include <iostream>
#include <sstream>

#include "lox/vm.hpp"

static std::string read_to_string(const char *path) {
  std::ifstream file;
  using ios = std::ios_base;
  file.exceptions(ios::badbit | ios::failbit | ios::eofbit);
  file.open(path);
  std::stringstream contents;
  contents << file.rdbuf();
  return contents.str();
}

void run_file(Lox::VirtualMachine &vm, const char *path) {
  std::string source = read_to_string(path);
  Lox::InterpretResult result = vm.interpret_source(source);
  if (result != Lox::InterpretResult::Ok) {
    std::exit(1);
  }
}

static void start_repl(Lox::VirtualMachine &vm) {
  std::string line;
  while (true) {
    std::cout << "> ";
    if (!std::getline(std::cin, line)) {
      std::cout << '\n';
      break;
    }
    vm.interpret_source(line);
  }
}

int main(int argc, const char **argv) {
  Lox::VirtualMachine vm;

  switch (argc) {
  case 1: start_repl(vm); break;
  case 2: run_file(vm, argv[1]); break;
  default: std::cerr << "Usage: " << argv[0] << " [path]\n";
  }
}
