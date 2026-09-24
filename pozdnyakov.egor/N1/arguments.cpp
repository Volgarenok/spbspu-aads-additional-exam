#include "arguments.hpp"
#include <string>

namespace {
  bool hasPrefix(const std::string & text, const std::string & prefix)
  {
    return text.size() > prefix.size() && text.compare(0, prefix.size(), prefix) == 0;
  }
}

bool pozdnyakov::parseArguments(int argc, const char * const * argv, Arguments & arguments)
{
  const std::string input_prefix = "in:";
  const std::string output_prefix = "out:";
  Arguments result{"", "", false, false};
  for (int i = 1; i < argc; ++i) {
    const std::string argument = argv[i];
    if (hasPrefix(argument, input_prefix) && !result.has_input) {
      result.input = argument.substr(input_prefix.size());
      result.has_input = true;
    } else if (hasPrefix(argument, output_prefix) && !result.has_output) {
      result.output = argument.substr(output_prefix.size());
      result.has_output = true;
    } else {
      return false;
    }
  }
  arguments = result;
  return true;
}
