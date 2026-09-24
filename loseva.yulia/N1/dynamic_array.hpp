#ifndef LOSEVA_DYNAMIC_ARRAY_HPP
#define LOSEVA_DYNAMIC_ARRAY_HPP

#include <cseddef>

namespace loseva {

template <typename T>
struct DynamicArray {
  T* data_;
  std::size_t capacity_;
  std::size_t size_;
};

template <typename T>
void initArray(DinamicArray<T> &arr) {
  arr.data_ = nullptr;
  arr.capacity_ = nullptr;
  arr.size_ = 0;
};

}

#endif
