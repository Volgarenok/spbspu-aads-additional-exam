#include <iostream>
#include "args.hpp"

int main(int argc, char* argv[]) {
  smirnova::Args args;
  const int parse_result = smirnova::parse_args(argc, argv, args);
  if (parse_result != 0) {
    return parse_result;
  }
  return 0;
}

