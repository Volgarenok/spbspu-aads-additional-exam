#include <fstream>
#include <iostream>
#include <istream>
#include <new>
#include <ostream>
#include "arguments.hpp"
#include "array.hpp"
#include "transaction-io.hpp"
#include "transaction.hpp"

int main(int argc, char ** argv)
{
  constexpr int max_arguments = 2;
  constexpr int invalid_arguments_code = 1;
  constexpr int file_error_code = 2;
  if (argc - 1 > max_arguments) {
    std::cerr << "Too many command line arguments\n";
    return 0;
  }
  pozdnyakov::Arguments arguments{"", "", false, false};
  if (!pozdnyakov::parseArguments(argc, argv, arguments)) {
    std::cerr << "Invalid command line arguments\n";
    return invalid_arguments_code;
  }

  pozdnyakov::Array< pozdnyakov::Transaction > transactions{nullptr, 0, 0};
  pozdnyakov::ReadStatistics statistics{0, 0};
  std::ifstream file;
  if (arguments.has_input) {
    file.open(arguments.input);
    if (!file.is_open()) {
      std::cerr << "Cannot open input file\n";
      return file_error_code;
    }
  }
  std::istream & input = arguments.has_input ? file : std::cin;
  try {
    statistics = pozdnyakov::readTransactions(input, transactions);
  } catch (const std::bad_alloc & error) {
    pozdnyakov::clear(transactions);
    std::cerr << "Not enough memory: " << error.what() << '\n';
    return file_error_code;
  }
  std::cerr << statistics.accepted << ' ' << statistics.ignored << '\n';

  std::ofstream output_file;
  if (arguments.has_output) {
    output_file.open(arguments.output);
    if (!output_file.is_open()) {
      std::cerr << "Cannot open output file\n";
      pozdnyakov::clear(transactions);
      return file_error_code;
    }
  }
  std::ostream & output = arguments.has_output ? output_file : std::cout;
  pozdnyakov::writeTransactions(output, transactions);
  if (transactions.size == 0) {
    output << '\n';
  }
  pozdnyakov::clear(transactions);
  return 0;
}
