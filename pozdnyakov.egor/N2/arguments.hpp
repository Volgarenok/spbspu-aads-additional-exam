#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include <string>

namespace pozdnyakov {
  struct Arguments {
    std::string input;
    std::string data;
    bool has_input;
  };

  bool parseArguments(int argc, const char * const * argv, Arguments & arguments);
}

#endif
