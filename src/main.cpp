#include <iostream>

void start_repl();
void run_file(const char *);

int main(int argc, const char **argv) {
  switch (argc) {
  case 1:
    start_repl();
    break;
  case 2:
    run_file(argv[1]);
    break;
  default:
    std::cerr << "Usage: " << argv[0] << " [path]\n";
    break;
  }
}
