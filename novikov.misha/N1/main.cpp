#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <args.h>
#include <dynamic_array.h>
#include <sorted_map.h>
#include <transaction.h>

namespace {

constexpr int k_success = 0;
constexpr int k_invalid_arguments = 1;
constexpr int k_cannot_open_file = 2;

struct ParsedArguments {
  std::string input_file;
  std::string output_file;
  bool has_input_file;
  bool has_output_file;
};

bool parseArguments(int argc, char *argv[], ParsedArguments &result) {
  novikov::DynamicArray<std::string> allowed_prefixes;
  allowed_prefixes.pushBack("in");
  allowed_prefixes.pushBack("out");

  novikov::SortedMap<std::string, std::string> named_args;
  if (!novikov::parseNamedArguments(argc, argv, allowed_prefixes, named_args)) {
    return false;
  }

  const std::string *input_file = named_args.find("in");
  if (input_file != nullptr) {
    result.input_file = *input_file;
    result.has_input_file = true;
  }
  const std::string *output_file = named_args.find("out");
  if (output_file != nullptr) {
    result.output_file = *output_file;
    result.has_output_file = true;
  }
  return true;
}

}

int main(int argc, char *argv[]) {
  ParsedArguments arguments{"", "", false, false};
  if (!parseArguments(argc, argv, arguments)) {
    std::cerr << "invalid command line arguments" << '\n';
    return k_invalid_arguments;
  }

  std::ifstream input_file_stream;
  if (arguments.has_input_file) {
    input_file_stream.open(arguments.input_file);
    if (!input_file_stream.is_open()) {
      std::cerr << "cannot open input file: " << arguments.input_file << '\n';
      return k_cannot_open_file;
    }
  }
  std::istream &input = arguments.has_input_file
                            ? static_cast<std::istream &>(input_file_stream)
                            : std::cin;

  novikov::DynamicArray<novikov::Transaction> transactions;
  std::size_t ignored_count = 0;
  novikov::readTransactions(input, transactions, ignored_count);

  std::ofstream output_file_stream;
  if (arguments.has_output_file) {
    output_file_stream.open(arguments.output_file);
    if (!output_file_stream.is_open()) {
      std::cerr << "cannot open output file: " << arguments.output_file << '\n';
      return k_cannot_open_file;
    }
  }
  std::ostream &output = arguments.has_output_file
                             ? static_cast<std::ostream &>(output_file_stream)
                             : std::cout;

  for (const novikov::Transaction &transaction : transactions) {
    novikov::writeTransaction(output, transaction);
  }

  std::cerr << transactions.size() << ' ' << ignored_count << '\n';

  return k_success;
}
