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

  std::ifstream file_in;
  if (has_in) {
    file_in.open(in_filename);
    if (!file_in.is_open()) {
      return 2;
    }
  }
  std::istream &input_stream = has_in ? file_in : std::cin;

  std::ofstream file_out;
  if (has_out) {
    file_out.open(out_filename);
    if (!file_out.is_open()) {
      return 2;
    }
  }
  std::ostream &output_stream = has_out ? file_out : std::cout;

  loseva::DynamicArray<loseva::Transaction> transactions;
  loseva::initArray(transactions);

  loseva::ReadStats stats;
  loseva::readTransactions(input_stream, transactions, stats);

  loseva::printTransactions(output_stream, transactions);
  std::cerr << stats.success_count_ << " " << stats.ignored_count_ << "\n";

  loseva::freeArray(transactions);
  return 0;
}

