#include "commission.hpp"
#include <cmath>

namespace loseva {

bool readCommissions(std::istream &in, DynamicArray<Commission> &commissions)
{
  int raw_acc = 0;
  int pct = 0;
  int st = 0;
  int en = 0;

  while (in >> raw_acc >> pct >> st >> en) {
    Commission c;
    c.account_ = std::abs(raw_acc);
    c.percent_ = pct;
    c.start_ = st;
    c.end_ = en;
    c.is_incoming_ = (raw_acc < 0);
    pushBack(commissions, c);
  }
  return true;
}

void calculateEffectiveIntervals(
  const DynamicArray<Commission> &commissions,
  const int account_id,
  const bool is_incoming,
  DynamicArray<EffectiveInterval> &result
)
{
  DynamicArray<int> points;
  initArray(points);

  for (std::size_t i = 0; i < commissions.size_; ++i) {
    const Commission &c = commissions.data_[i];
    if (c.account_ == account_id && c.is_incoming_ == is_incoming) {
      pushBack(points, c.start_);
      pushBack(points, c.end_);
    }
  }

  if (points.size_ == 0) {
    freeArray(points);
    return;
  }

  for (std::size_t i = 0; i < points.size_; ++i) {
    for (std::size_t j = i + 1; j < points.size_; ++j) {
      if (points.data_[i] > points.data_[j]) {
        const int temp = points.data_[i];
        points.data_[i] = points.data_[j];
        points.data_[j] = temp;
      }
    }
  }

  for (std::size_t i = 0; i < points.size_ - 1; ++i) {
    const int seg_start = points.data_[i];
    const int seg_end = points.data_[i + 1];
    if (seg_start == seg_end) {
      continue;
    }

    int sum_pct = 0;
    for (std::size_t k = 0; k < commissions.size_; ++k) {
      const Commission &c = commissions.data_[k];
      if (c.account_ == account_id && c.is_incoming_ == is_incoming) {
        if (c.start_ <= seg_start && c.end_ >= seg_end) {
          sum_pct += c.percent_;
        }
      }
    }

    if (sum_pct > 0) {
      EffectiveInterval seg;
      seg.start_ = seg_start;
      seg.end_ = seg_end;
      seg.total_percent_ = sum_pct;
      pushBack(result, seg);
    }
  }

  freeArray(points);
}

}

