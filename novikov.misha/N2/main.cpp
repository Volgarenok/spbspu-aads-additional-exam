#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <args.h>
#include <bank.h>
#include <commission.h>
#include <dynamic_array.h>
#include <sorted_map.h>
#include <transaction.h>

namespace {

constexpr int kSuccess = 0;
constexpr int kInvalidArguments = 1;
constexpr int kCannotOpenFile = 2;
constexpr int kReadError = 3;

const char *kInvalidCommand = "<INVALID COMMAND>";

struct ParsedArguments
{
  std::string dataFile;
  std::string inputFile;
  bool hasInputFile;
};

bool parseArguments(int argc, char *argv[], ParsedArguments &result)
{
  novikov::DynamicArray<std::string> allowedPrefixes;
  allowedPrefixes.pushBack("data");
  allowedPrefixes.pushBack("in");

  novikov::SortedMap<std::string, std::string> named;
  if (!novikov::parseNamedArguments(argc, argv, allowedPrefixes, named)) {
    return false;
  }

  const std::string *data = named.find("data");
  if (data == nullptr) {
    return false;
  }
  result.dataFile = *data;

  const std::string *input = named.find("in");
  if (input != nullptr) {
    result.inputFile = *input;
    result.hasInputFile = true;
  }
  return true;
}

void tokenize(const std::string &line,
              novikov::DynamicArray<std::string> &tokens)
{
  std::size_t i = 0;
  const std::size_t n = line.size();
  while (i < n) {
    while (i < n &&
           std::isspace(static_cast<unsigned char>(line[i])) != 0) {
      ++i;
    }
    if (i >= n) {
      break;
    }
    std::size_t start = i;
    while (i < n &&
           std::isspace(static_cast<unsigned char>(line[i])) == 0) {
      ++i;
    }
    tokens.pushBack(line.substr(start, i - start));
  }
}

bool parseInt(const std::string &s, int &out)
{
  try {
    std::size_t pos = 0;
    out = std::stoi(s, &pos);
    return pos == s.size();
  } catch (...) {
    return false;
  }
}

void printIntervals(
    const novikov::DynamicArray<novikov::CommissionInterval> &intervals)
{
  for (std::size_t i = 0; i < intervals.size(); ++i) {
    std::cout << intervals[i].from << ' '
              << intervals[i].to << ' '
              << intervals[i].percent << '\n';
  }
}

void processCommand(const std::string &line, novikov::Bank &bank)
{
  novikov::DynamicArray<std::string> tokens;
  tokenize(line, tokens);
  if (tokens.size() == 0) {
    return;
  }

  const std::string &cmd = tokens[0];

  if ((cmd == "list-in" || cmd == "list-out") && tokens.size() == 2) {
    int id = 0;
    if (!parseInt(tokens[1], id) || id <= 0 || !bank.hasAccount(id)) {
      std::cout << kInvalidCommand << '\n';
      return;
    }
    novikov::DynamicArray<novikov::Transaction> list =
        (cmd == "list-in") ? bank.listIn(id) : bank.listOut(id);
    for (std::size_t i = 0; i < list.size(); ++i) {
      novikov::writeTransaction(std::cout, list[i]);
    }
    return;
  }

  if ((cmd == "income" || cmd == "outcome") && tokens.size() == 2) {
    int id = 0;
    if (!parseInt(tokens[1], id) || id <= 0 || !bank.hasAccount(id)) {
      std::cout << kInvalidCommand << '\n';
      return;
    }
    long long result =
        (cmd == "income") ? bank.income(id) : bank.outcome(id);
    std::cout << result << '\n';
    return;
  }

  if ((cmd == "bills-in" || cmd == "bills-out") && tokens.size() == 2) {
    int id = 0;
    if (!parseInt(tokens[1], id) || id <= 0 || !bank.hasAccount(id)) {
      std::cout << kInvalidCommand << '\n';
      return;
    }
    novikov::DynamicArray<novikov::CommissionInterval> intervals =
        (cmd == "bills-in") ? bank.billsIn(id) : bank.billsOut(id);
    printIntervals(intervals);
    return;
  }

  if ((cmd == "pay-bills-in" || cmd == "pay-bills-out") &&
      tokens.size() == 4) {
    int id = 0;
    int from = 0;
    int to = 0;
    if (!parseInt(tokens[1], id) || !parseInt(tokens[2], from) ||
        !parseInt(tokens[3], to)) {
      std::cout << kInvalidCommand << '\n';
      return;
    }
    if (id <= 0 || from > to || !bank.hasAccount(id)) {
      std::cout << kInvalidCommand << '\n';
      return;
    }
    if (cmd == "pay-bills-in") {
      bank.payBillsIn(id, from, to);
    } else {
      bank.payBillsOut(id, from, to);
    }
    return;
  }

  if (cmd == "out-transactions" && tokens.size() == 2) {
    std::ofstream out(tokens[1]);
    if (!out.is_open()) {
      std::cerr << "cannot open output file: " << tokens[1] << '\n';
      return;
    }
    bank.writeTransactions(out);
    return;
  }

  std::cout << kInvalidCommand << '\n';
}

bool looksLikeCommand(const std::string &line)
{
  novikov::DynamicArray<std::string> tokens;
  tokenize(line, tokens);
  if (tokens.size() == 0) {
    return false;
  }
  const std::string &t = tokens[0];
  return t == "list-in" || t == "list-out" || t == "income" ||
         t == "outcome" || t == "bills-in" || t == "bills-out" ||
         t == "pay-bills-in" || t == "pay-bills-out" ||
         t == "out-transactions";
}

}

int main(int argc, char *argv[])
{
  ParsedArguments arguments{"", "", false};
  if (!parseArguments(argc, argv, arguments)) {
    std::cerr << "invalid command line arguments\n";
    return kInvalidArguments;
  }

  std::ifstream dataStream(arguments.dataFile);
  if (!dataStream.is_open()) {
    std::cerr << "cannot open data file: " << arguments.dataFile << '\n';
    return kCannotOpenFile;
  }

  novikov::Bank bank;
  novikov::DynamicArray<novikov::Commission> commissions;
  if (!novikov::readCommissions(dataStream, commissions)) {
    std::cerr << "invalid commissions data\n";
    return kReadError;
  }
  for (std::size_t i = 0; i < commissions.size(); ++i) {
    bank.addCommission(commissions[i]);
  }

  if (arguments.hasInputFile) {
    std::ifstream inputStream(arguments.inputFile);
    if (!inputStream.is_open()) {
      std::cerr << "cannot open input file: " << arguments.inputFile << '\n';
      return kCannotOpenFile;
    }
    std::string line;
    while (std::getline(inputStream, line)) {
      novikov::Transaction t{0, 0, 0};
      if (!novikov::parseTransactionLine(line, t)) {
        std::cerr << "invalid transaction line\n";
        return kReadError;
      }
      if (t.from == t.to) {
        continue;
      }
      bank.addTransaction(t);
    }
  }

  std::string line;
  while (std::getline(std::cin, line)) {
    if (!arguments.hasInputFile && !looksLikeCommand(line)) {
      novikov::Transaction t{0, 0, 0};
      if (novikov::parseTransactionLine(line, t)) {
        if (t.from != t.to) {
          bank.addTransaction(t);
        }
        continue;
      }
    }
    processCommand(line, bank);
  }

  return kSuccess;
}
