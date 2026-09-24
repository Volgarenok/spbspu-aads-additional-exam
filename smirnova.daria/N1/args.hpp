#ifndef ARGS_HPP
#define ARGS_HPP

#include <string>

namespace smirnova {
  struct Args {
    std::string in_file;
    std::string out_file;
    bool has_in;
    bool has_out;
  };

  int parse_args(const int argc, char* argv[], Args& args);
}

#endif
