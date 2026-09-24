#ifndef COMMON_SORTED_MAP_H_
#define COMMON_SORTED_MAP_H_

#include <cstddef>
#include <utility>

#include "dynamic_array.h"

namespace novikov {

template <typename K, typename V>
class SortedMap
{
public:
  void insert(const K &key, const V &value)
  {
    std::size_t i = 0;
    while (i < entries_.size() && entries_[i].first < key) {
      ++i;
    }
    if (i < entries_.size() && !(key < entries_[i].first) && !(entries_[i].first < key)) {
      entries_[i].second = value;
      return;
    }
    entries_.pushBack(std::pair<K, V>(K(), V()));
    for (std::size_t j = entries_.size() - 1; j > i; --j) {
      entries_[j] = entries_[j - 1];
    }
    entries_[i] = std::pair<K, V>(key, value);
  }

  const V *find(const K &key) const
  {
    for (std::size_t i = 0; i < entries_.size(); ++i) {
      if (!(entries_[i].first < key) && !(key < entries_[i].first)) {
        return &entries_[i].second;
      }
      if (key < entries_[i].first) {
        break;
      }
    }
    return nullptr;
  }

  std::size_t size() const
  {
    return entries_.size();
  }

  bool empty() const
  {
    return entries_.empty();
  }

private:
  DynamicArray<std::pair<K, V>> entries_;
};

}

#endif
