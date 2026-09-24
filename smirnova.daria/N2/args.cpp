#include "args.hpp"

namespace smirnova {
  int parse_args(const int argc, char* argv[], Args& args) {
    args.has_in = false;
    args.has_out = false;

    if (argc > 3) {
      return 1;
    }

    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];

      if (arg.compare(0, 3, "in:") == 0) {
        if (args.has_in || arg.size() == 3) {
          return 1;
        }
        args.in_file = arg.substr(3);
        args.has_in = true;
      } else if (arg.compare(0, 4, "out:") == 0) {
        if (args.has_out || arg.size() == 4) {
          return 1;
        }
        args.out_file = arg.substr(4);
        args.has_out = true;
      } else {
        return 1;
      }
    }

    return 0;
  }
}
