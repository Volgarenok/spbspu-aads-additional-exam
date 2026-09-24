#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <cstddef>

namespace loseva {

template <typename T>
struct DynamicArray {
  T *data;
  std::size_t capacity;
  std::size_t size;
};

template <typename T>
void initArray(DynamicArray<T> &arr)
{
  arr.data = nullptr;
  arr.capacity = 0;
  arr.size = 0;
}

template <typename T>
void freeArray(DynamicArray<T> &arr)
{
  delete[] arr.data;
  arr.data = nullptr;
  arr.capacity = 0;
  arr.size = 0;
}

template <typename T>
void pushBack(DynamicArray<T> &arr, const T &value)
{
  if (arr.size >= arr.capacity) {
    const std::size_t default_cap = 4;
    const std::size_t new_cap = (arr.capacity == 0) ? default_cap : (arr.capacity * 2);
    T * const new_data = new T[new_cap];
    for (std::size_t i = 0; i < arr.size; ++i) {
      new_data[i] = arr.data[i];
    }
    delete[] arr.data;
    arr.data = new_data;
    arr.capacity = new_cap;
  }
  arr.data[arr.size] = value;
  arr.size++;
}

}

#endif

