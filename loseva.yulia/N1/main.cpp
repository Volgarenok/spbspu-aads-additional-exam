#include <fstream>
#include <iostream>
#include <string>
#include "transaction.hpp"

int main(const int argc, const char * const argv[])
{
  if (argc > 3) {
    return 1;
  }

  std::string in_filename;
  std::string out_filename;
  bool has_in = false;
  bool has_out = false;

  for (int i = 1; i < argc; ++i) {
    const std::string arg(argv[i]);
    if (arg.substr(0, 3) == "in:") {
      if (has_in) {
        return 1;
      }
      in_filename = arg.substr(3);
      has_in = true;
    } else if (arg.substr(0, 4) == "out:") {
      if (has_out) {
        return 1;
      }
      out_filename = arg.substr(4);
      has_out = true;
    } else {
      return 1;
    }
  }
}

