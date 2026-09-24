#ifndef LOSEVA_DYNAMIC_ARRAY_HPP
#define LOSEVA_DYNAMIC_ARRAY_HPP

#include <cstddef>

namespace loseva {

template <typename T>
struct DynamicArray {
  T *data_;
  std::size_t capacity_;
  std::size_t size_;
};

template <typename T>
void initArray(DynamicArray<T> &arr)
{
  arr.data_ = nullptr;
  arr.capacity_ = 0;
  arr.size_ = 0;
}

template <typename T>
void freeArray(DynamicArray<T> &arr)
{
  delete[] arr.data_;
  arr.data_ = nullptr;
  arr.capacity_ = 0;
  arr.size_ = 0;
}

template <typename T>
void pushBack(DynamicArray<T> &arr, const T &value)
{
  if (arr.size_ >= arr.capacity_) {
    const std::size_t new_cap = (arr.capacity_ == 0) ? 4 : (arr.capacity_ * 2);
    T *new_data = new T[new_cap];
    for (std::size_t i = 0; i < arr.size_; ++i) {
      new_data[i] = arr.data_[i];
    }
    delete[] arr.data_;
    arr.data_ = new_data;
    arr.capacity_ = new_cap;
  }
  arr.data_[arr.size_] = value;
  arr.size_++;
}

}

#endif

