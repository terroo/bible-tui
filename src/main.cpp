#include "options.hpp"

int main(int argc, char** argv){
  auto op = std::make_unique<Options>();
  op->run_options(argc, argv);
}

