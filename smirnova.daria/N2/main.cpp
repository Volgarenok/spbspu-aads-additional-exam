#include <iostream>
#include <fstream>
#include "transaction.hpp"
#include "args.hpp"

int main(int argc, char* argv[]) {
  smirnova::Args args;
  const int parse_result = smirnova::parse_args(argc, argv, args);
  if (parse_result != 0) {
    return parse_result;
  }

  std::ifstream in_file;
  if (args.has_in) {
    in_file.open(args.in_file);
    if (!in_file.is_open()) {
        return 2;
    }
  }

  std::istream& in = args.has_in ? static_cast<std::istream&>(in_file) : std::cin;

  smirnova::Array<smirnova::Transaction> transactions = smirnova::array_create< smirnova::Transaction >();
  std::size_t ignored = 0;

  smirnova::read_transactions(in, transactions, ignored);

  if (args.has_in) {
    in_file.close();
  }

  std::ofstream out_file;
  if (args.has_out) {
    out_file.open(args.out_file);
    if (!out_file.is_open()) {
      smirnova::array_destroy(transactions);
      return 2;
    }
  }

  std::ostream& out = args.has_out ? static_cast<std::ostream&>(out_file) : std::cout;

  smirnova::write_transactions(out, transactions);
  std::cerr << transactions.size << ' ' << ignored << '\n';

  smirnova::array_destroy(transactions);
  return 0;
}
