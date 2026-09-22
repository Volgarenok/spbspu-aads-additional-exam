#ifndef SMIRNOVA_ARRAY_HPP
#define SMIRNOVA_ARRAY_HPP

#include <cstddef>
#include <cstdlib>

namespace smirnova {
  template <typename T>
  struct Array {
    T* data;
    std::size_t size;
    std::size_t capacity;
  };

  template <typename T>
  Array<T> array_create() {
    return Array<T>{nullptr, 0, 0};
  }

  template <typename T>
  void array_reserve(Array<T>& arr, std::size_t new_cap) {
    if (new_cap <= arr.capacity) return;
    T* new_data = static_cast<T*>(std::malloc(new_cap * sizeof(T)));
    for (std::size_t i = 0; i < arr.size; ++i) {
      new_data[i] = arr.data[i];
    }
    std::free(arr.data);
    arr.data = new_data;
    arr.capacity = new_cap;
  }

  template <typename T>
  void array_push(Array<T>& arr, const T& val) {
    if (arr.size == arr.capacity) {
      if (arr.capacity == 0) {
          array_reserve(arr, 8);
      } else {
        array_reserve(arr, arr.capacity * 2);
      }
    }
    arr.data[arr.size] = val;
    ++arr.size;
  }

  template <typename T>
  void array_destroy(Array<T>& arr) {
    std::free(arr.data);
    arr.data = nullptr;
    arr.size = 0;
    arr.capacity = 0;
  }
}

#endif
