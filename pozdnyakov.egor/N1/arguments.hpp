#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include <string>

namespace pozdnyakov {
  struct Arguments {
    std::string input;
    std::string output;
    bool has_input;
    bool has_output;
  };

  bool parseArguments(int argc, const char * const * argv, Arguments & arguments);
}

#endif
