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
  constexpr int max_arguments = 2;
  if (argc <= 1 || argc - 1 > max_arguments) {
    return false;
  }
  const std::string input_prefix = "in:";
  const std::string data_prefix = "data:";
  Arguments result{"", "", false};
  bool has_data = false;
  for (int i = 1; i < argc; ++i) {
    const std::string argument = argv[i];
    if (hasPrefix(argument, input_prefix) && !result.has_input) {
      result.input = argument.substr(input_prefix.size());
      result.has_input = true;
    } else if (hasPrefix(argument, data_prefix) && !has_data) {
      result.data = argument.substr(data_prefix.size());
      has_data = true;
    } else {
      return false;
    }
  }
  if (!has_data) {
    return false;
  }
  arguments = result;
  return true;
}
