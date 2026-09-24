#ifndef COMMON_BANK_H_
#define COMMON_BANK_H_

#include <cstddef>
#include <cstdlib>
#include <ostream>

#include <commission.h>
#include <dynamic_array.h>
#include <transaction.h>

namespace novikov {

class Bank
{
public:
  Bank()
    : transactions_()
    , commissions_()
    , paidRanges_()
  {
  }

  void addTransaction(const Transaction &t)
  {
    transactions_.pushBack(t);
  }

  void addCommission(const Commission &c)
  {
    commissions_.pushBack(c);
  }

  const DynamicArray<Transaction> &transactions() const
  {
    return transactions_;
  }

  bool hasAccount(int account) const
  {
    for (std::size_t i = 0; i < transactions_.size(); ++i) {
      if (transactions_[i].from == account ||
          transactions_[i].to == account) {
        return true;
      }
    }
    for (std::size_t i = 0; i < commissions_.size(); ++i) {
      if (std::abs(commissions_[i].account) == account) {
        return true;
      }
    }
    return false;
  }

  DynamicArray<Transaction> listIn(int account) const
  {
    DynamicArray<Transaction> result;
    for (std::size_t i = 0; i < transactions_.size(); ++i) {
      if (transactions_[i].to == account) {
        result.pushBack(transactions_[i]);
      }
    }
    sortDescByValue(result);
    return result;
  }

  DynamicArray<Transaction> listOut(int account) const
  {
    DynamicArray<Transaction> result;
    for (std::size_t i = 0; i < transactions_.size(); ++i) {
      if (transactions_[i].from == account) {
        result.pushBack(transactions_[i]);
      }
    }
    sortDescByValue(result);
    return result;
  }

  long long income(int account) const
  {
    long long total = 0;
    for (std::size_t i = 0; i < transactions_.size(); ++i) {
      if (transactions_[i].to == account) {
        total += transactions_[i].value;
      }
    }
    return total;
  }

  long long outcome(int account) const
  {
    long long total = 0;
    for (std::size_t i = 0; i < transactions_.size(); ++i) {
      if (transactions_[i].from == account) {
        total += transactions_[i].value;
      }
    }
    return total;
  }

  DynamicArray<CommissionInterval> billsIn(int account) const
  {
    DynamicArray<CommissionInterval> all =
        mergeCommissionsForAccount(commissions_, -account);
    DynamicArray<CommissionInterval> paid = paidForAccount(-account);
    return subtractPaidIntervals(all, paid);
  }

  DynamicArray<CommissionInterval> billsOut(int account) const
  {
    DynamicArray<CommissionInterval> all =
        mergeCommissionsForAccount(commissions_, account);
    DynamicArray<CommissionInterval> paid = paidForAccount(account);
    return subtractPaidIntervals(all, paid);
  }

  long long payBillsOut(int account, int from, int to)
  {
    DynamicArray<CommissionInterval> all =
        mergeCommissionsForAccount(commissions_, account);
    DynamicArray<CommissionInterval> paid = paidForAccount(account);
    DynamicArray<CommissionInterval> unpaid = subtractPaidIntervals(all, paid);
    long long amount = computeCommissionAmount(unpaid, from, to);
    if (amount > 0) {
      addTransaction(Transaction{account, 0, static_cast<int>(amount)});
    }
    markPaid(account, from, to);
    return amount;
  }

  long long payBillsIn(int account, int from, int to)
  {
    DynamicArray<CommissionInterval> all =
        mergeCommissionsForAccount(commissions_, -account);
    DynamicArray<CommissionInterval> paid = paidForAccount(-account);
    DynamicArray<CommissionInterval> unpaid = subtractPaidIntervals(all, paid);
    long long amount = computeCommissionAmount(unpaid, from, to);
    if (amount > 0) {
      addTransaction(Transaction{account, 0, static_cast<int>(amount)});
    }
    markPaid(-account, from, to);
    return amount;
  }

  void writeTransactions(std::ostream &out) const
  {
    for (std::size_t i = 0; i < transactions_.size(); ++i) {
      writeTransaction(out, transactions_[i]);
    }
  }

private:
  struct PaidRange
  {
    int account;
    int from;
    int to;
  };

  static void sortDescByValue(DynamicArray<Transaction> &arr)
  {
    for (std::size_t i = 1; i < arr.size(); ++i) {
      Transaction key = arr[i];
      std::size_t j = i;
      while (j > 0 && arr[j - 1].value < key.value) {
        arr[j] = arr[j - 1];
        --j;
      }
      arr[j] = key;
    }
  }

  DynamicArray<CommissionInterval> paidForAccount(int signedAccount) const
  {
    DynamicArray<CommissionInterval> result;
    for (std::size_t i = 0; i < paidRanges_.size(); ++i) {
      if (paidRanges_[i].account == signedAccount) {
        result.pushBack(CommissionInterval{
            paidRanges_[i].from, paidRanges_[i].to, 0});
      }
    }
    return result;
  }

  void markPaid(int signedAccount, int from, int to)
  {
    paidRanges_.pushBack(PaidRange{signedAccount, from, to});
  }

  DynamicArray<Transaction> transactions_;
  DynamicArray<Commission> commissions_;
  DynamicArray<PaidRange> paidRanges_;
};

}

#endif
