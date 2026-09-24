#ifndef BANK_DATA_HPP
#define BANK_DATA_HPP

#include <istream>
#include <string>
#include <array.hpp>
#include <hash-table.hpp>
#include <transaction.hpp>

namespace pozdnyakov {
  struct Commission {
    int account;
    bool incoming;
    int percent;
    int from, to;
  };

  struct AccountTotals {
    long long income;
    long long outcome;
  };

  struct BankData {
    Array< Transaction > transactions;
    Array< Commission > commissions;
    HashTable< int, AccountTotals > accounts;
  };

  void addTransaction(BankData & data, const Transaction & transaction);
  bool readTransactions(std::istream & in, BankData & data, std::string & first_command);
  bool readCommissions(std::istream & in, BankData & data);
  void clear(BankData & data) noexcept;
}

#endif
