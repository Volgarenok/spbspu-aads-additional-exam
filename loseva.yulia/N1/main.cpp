#include <fstream>
#include <iostream>
#include <string>
#include "dynamic_array.hpp"
#include "transaction.hpp"

namespace {
constexpr int EXIT_INVALID_ARGS = 1;
constexpr int EXIT_FILE_ERROR = 2;
constexpr int MAX_CLI_ARGS = 3;
constexpr std::size_t IN_PREFIX_LEN = 3;
constexpr std::size_t OUT_PREFIX_LEN = 4;
}

int main(const int argc, const char * const argv[])
{
  if (argc > MAX_CLI_ARGS) {
    return EXIT_INVALID_ARGS;
  }

  std::string in_filename;
  std::string out_filename;
  bool has_in = false;
  bool has_out = false;

  for (int i = 1; i < argc; ++i) {
    const std::string arg(argv[i]);
    if (arg.substr(0, IN_PREFIX_LEN) == "in:") {
      if (has_in) {
        return EXIT_INVALID_ARGS;
      }
      in_filename = arg.substr(IN_PREFIX_LEN);
      has_in = true;
    } else if (arg.substr(0, OUT_PREFIX_LEN) == "out:") {
      if (has_out) {
        return EXIT_INVALID_ARGS;
      }
      out_filename = arg.substr(OUT_PREFIX_LEN);
      has_out = true;
    } else {
      return EXIT_INVALID_ARGS;
    }
  }

  loseva::DynamicArray<loseva::Transaction> transactions;
  loseva::initArray(transactions);
  loseva::ReadStats stats{0, 0};

  if (has_in) {
    std::ifstream file_in(in_filename);
    if (!file_in.is_open()) {
      loseva::freeArray(transactions);
      return EXIT_FILE_ERROR;
    }
    loseva::readTransactions(file_in, transactions, stats);
  } else {
    loseva::readTransactions(std::cin, transactions, stats);
  }

  if (has_out) {
    std::ofstream file_out(out_filename);
    if (!file_out.is_open()) {
      loseva::freeArray(transactions);
      return EXIT_FILE_ERROR;
    }
    loseva::printTransactions(file_out, transactions);
  } else {
    loseva::printTransactions(std::cout, transactions);
  }

  std::cerr << stats.success_count << " " << stats.ignored_count << "\n";

  loseva::freeArray(transactions);
  return 0;
}

