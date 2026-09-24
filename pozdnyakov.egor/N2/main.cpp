#include <fstream>
#include <iostream>
#include <istream>
#include <new>
#include <string>
#include <tree.hpp>
#include "arguments.hpp"
#include "bank-data.hpp"
#include "commands.hpp"

int main(int argc, char ** argv)
{
  constexpr int invalid_arguments_code = 1;
  constexpr int file_error_code = 2;
  constexpr int input_error_code = 3;
  pozdnyakov::Arguments arguments{"", "", false};
  if (!pozdnyakov::parseArguments(argc, argv, arguments)) {
    std::cerr << "Invalid command line arguments\n";
    return invalid_arguments_code;
  }
  std::ifstream data_file(arguments.data);
  if (!data_file.is_open()) {
    std::cerr << "Cannot open data file\n";
    return file_error_code;
  }
  std::ifstream input_file;
  if (arguments.has_input) {
    input_file.open(arguments.input);
    if (!input_file.is_open()) {
      std::cerr << "Cannot open input file\n";
      return file_error_code;
    }
  }
  std::istream & input = arguments.has_input ? input_file : std::cin;

  pozdnyakov::BankData data{{nullptr, 0, 0}, {nullptr, 0, 0}, {nullptr, 0, 0}};
  pozdnyakov::CommandTree commands{nullptr, 0};
  int code = 0;
  try {
    std::string first_command;
    bool is_read = pozdnyakov::readTransactions(input, data, first_command);
    is_read = is_read && (!arguments.has_input || first_command.empty());
    is_read = is_read && pozdnyakov::readCommissions(data_file, data);
    if (is_read) {
      pozdnyakov::makeCommands(commands);
      pozdnyakov::runCommand(commands, data, first_command, std::cout);
      std::string line;
      while (std::getline(std::cin, line)) {
        pozdnyakov::runCommand(commands, data, line, std::cout);
      }
    } else {
      std::cerr << "Invalid input data\n";
      code = input_error_code;
    }
  } catch (const std::bad_alloc & error) {
    std::cerr << "Not enough memory: " << error.what() << '\n';
    code = file_error_code;
  }
  pozdnyakov::clear(commands);
  pozdnyakov::clear(data);
  return code;
}
