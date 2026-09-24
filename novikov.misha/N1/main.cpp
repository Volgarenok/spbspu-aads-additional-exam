#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <common/args.h>
#include <common/dynamic_array.h>
#include <common/sorted_map.h>
#include <common/transaction.h>

namespace {

constexpr int kSuccess = 0;
constexpr int kInvalidArguments = 1;
constexpr int kCannotOpenFile = 2;

struct ParsedArguments
{
  std::string inputFile;
  std::string outputFile;
  bool hasInputFile;
  bool hasOutputFile;
};

bool parseArguments(int argc, char *argv[], ParsedArguments &result)
{
  novikov::DynamicArray<std::string> allowedPrefixes;
  allowedPrefixes.pushBack("in");
  allowedPrefixes.pushBack("out");

  novikov::SortedMap<std::string, std::string> namedArgs;
  if (!novikov::parseNamedArguments(argc, argv, allowedPrefixes, namedArgs)) {
    return false;
  }

  if (const std::string *inputFile = namedArgs.find("in")) {
    result.inputFile = *inputFile;
    result.hasInputFile = true;
  }
  if (const std::string *outputFile = namedArgs.find("out")) {
    result.outputFile = *outputFile;
    result.hasOutputFile = true;
  }
  return true;
}

}

int main(int argc, char *argv[])
{
  ParsedArguments arguments{"", "", false, false};
  if (!parseArguments(argc, argv, arguments)) {
    std::cerr << "invalid command line arguments" << '\n';
    return kInvalidArguments;
  }

  std::ifstream inputFileStream;
  if (arguments.hasInputFile) {
    inputFileStream.open(arguments.inputFile);
    if (!inputFileStream.is_open()) {
      std::cerr << "cannot open input file: " << arguments.inputFile << '\n';
      return kCannotOpenFile;
    }
  }
  std::istream &input = arguments.hasInputFile ? static_cast<std::istream &>(inputFileStream) : std::cin;

  novikov::DynamicArray<novikov::Transaction> transactions;
  std::size_t ignoredCount = 0;
  novikov::readTransactions(input, transactions, ignoredCount);

  std::ofstream outputFileStream;
  if (arguments.hasOutputFile) {
    outputFileStream.open(arguments.outputFile);
    if (!outputFileStream.is_open()) {
      std::cerr << "cannot open output file: " << arguments.outputFile << '\n';
      return kCannotOpenFile;
    }
  }
  std::ostream &output = arguments.hasOutputFile ? static_cast<std::ostream &>(outputFileStream) : std::cout;

  for (const novikov::Transaction &transaction : transactions) {
    novikov::writeTransaction(output, transaction);
  }

  std::cerr << transactions.size() << ' ' << ignoredCount << '\n';

  return kSuccess;
}
