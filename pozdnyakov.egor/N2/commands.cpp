#include "commands.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <ostream>
#include <string>
#include <utility>
#include <array.hpp>
#include <hash-table.hpp>
#include <parsing.hpp>
#include <transaction-io.hpp>
#include <transaction.hpp>
#include <tree.hpp>
#include "bank-data.hpp"

namespace pozdnyakov {
  namespace {
    const AccountTotals * readAccount(const BankData & data, const std::string & line, int & account)
    {
      std::size_t position = 0;
      int id = 0;
      if (!parseInteger(line, position, id) || id < 0 || !isLineEnd(line, position)) {
        return nullptr;
      }
      account = id;
      return find(data.accounts, id);
    }

    bool isGreaterValue(const Transaction & lhs, const Transaction & rhs)
    {
      return lhs.value > rhs.value;
    }

    void listTransactions(const BankData & data, int account, bool incoming, std::ostream & out)
    {
      Array< Transaction > selected{nullptr, 0, 0};
      try {
        for (std::size_t i = 0; i < data.transactions.size; ++i) {
          const Transaction & transaction = data.transactions.data[i];
          if ((incoming ? transaction.to : transaction.from) == account) {
            pushBack(selected, transaction);
          }
        }
        std::stable_sort(selected.data, selected.data + selected.size, isGreaterValue);
      } catch (...) {
        clear(selected);
        throw;
      }
      writeTransactions(out, selected);
      clear(selected);
    }

    bool listIn(BankData & data, const std::string & arguments, std::ostream & out)
    {
      int account = 0;
      if (readAccount(data, arguments, account) == nullptr) {
        return false;
      }
      listTransactions(data, account, true, out);
      return true;
    }

    bool listOut(BankData & data, const std::string & arguments, std::ostream & out)
    {
      int account = 0;
      if (readAccount(data, arguments, account) == nullptr) {
        return false;
      }
      listTransactions(data, account, false, out);
      return true;
    }

    bool income(BankData & data, const std::string & arguments, std::ostream & out)
    {
      int account = 0;
      const AccountTotals * const totals = readAccount(data, arguments, account);
      if (totals == nullptr) {
        return false;
      }
      out << totals->income << '\n';
      return true;
    }

    bool outcome(BankData & data, const std::string & arguments, std::ostream & out)
    {
      int account = 0;
      const AccountTotals * const totals = readAccount(data, arguments, account);
      if (totals == nullptr) {
        return false;
      }
      out << totals->outcome << '\n';
      return true;
    }

    bool outTransactions(BankData & data, const std::string & arguments, std::ostream & out)
    {
      static_cast< void >(out);
      std::size_t position = 0;
      const std::string filename = readToken(arguments, position);
      if (filename.empty() || !isLineEnd(arguments, position)) {
        return false;
      }
      std::ofstream file(filename);
      if (!file.is_open()) {
        return false;
      }
      writeTransactions(file, data.transactions);
      return true;
    }
  }
}

void pozdnyakov::makeCommands(CommandTree & commands)
{
  using CommandEntry = std::pair< std::string, Command >;
  const CommandEntry entries[] = {
      {"list-in", listIn},
      {"list-out", listOut},
      {"income", income},
      {"outcome", outcome},
      {"out-transactions", outTransactions},
  };
  for (const CommandEntry & entry : entries) {
    insert(commands, entry.first, entry.second);
  }
}

void pozdnyakov::runCommand(const CommandTree & tree, BankData & data, const std::string & line, std::ostream & out)
{
  if (isBlank(line)) {
    return;
  }
  std::size_t position = 0;
  const std::string name = readToken(line, position);
  const Command * const command = find(tree, name);
  if (command == nullptr || !(*command)(data, line.substr(position), out)) {
    out << "<INVALID COMMAND>\n";
  }
}
