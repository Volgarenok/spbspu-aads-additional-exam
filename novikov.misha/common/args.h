#ifndef ARGS_H
#define ARGS_H

#include <cstddef>
#include <string>

#include <dynamic_array.h>
#include <sorted_map.h>

namespace novikov {

inline bool isAllowedPrefix(const std::string &prefix,
                            const DynamicArray<std::string> &allowed) {
  for (std::size_t i = 0; i < allowed.size(); ++i) {
    if (allowed[i] == prefix) {
      return true;
    }
  }
  return false;
}

inline bool
parseNamedArguments(int argc, char *argv[],
                    const DynamicArray<std::string> &allowed_prefixes,
                    SortedMap<std::string, std::string> &result) {
  constexpr int k_max_args = 2;
  if (argc - 1 > k_max_args) {
    return false;
  }

  for (int i = 1; i < argc; ++i) {
    const std::string argument = argv[i];
    const std::size_t colon = argument.find(':');
    if (colon == std::string::npos) {
      return false;
    }

    const std::string prefix = argument.substr(0, colon);
    const std::string value = argument.substr(colon + 1);
    if (prefix.empty() || value.empty()) {
      return false;
    }
    if (!isAllowedPrefix(prefix, allowed_prefixes)) {
      return false;
    }
    if (result.find(prefix) != nullptr) {
      return false;
    }

    result.insert(prefix, value);
  }

  return true;
}

}

#endif
