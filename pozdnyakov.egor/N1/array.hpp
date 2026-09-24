#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cstddef>

namespace pozdnyakov {
  template< class T >
  struct Array {
    T * data;
    std::size_t size;
    std::size_t capacity;
  };

  template< class T >
  void pushBack(Array< T > & array, const T & value)
  {
    if (array.size < array.capacity) {
      array.data[array.size] = value;
      ++array.size;
      return;
    }
    constexpr std::size_t initial_capacity = 16;
    constexpr std::size_t growth_factor = 2;
    const std::size_t new_capacity = array.capacity == 0 ? initial_capacity : array.capacity * growth_factor;
    T * const new_data = new T[new_capacity];
    try {
      for (std::size_t i = 0; i < array.size; ++i) {
        new_data[i] = array.data[i];
      }
      new_data[array.size] = value;
    } catch (...) {
      delete[] new_data;
      throw;
    }
    delete[] array.data;
    array.data = new_data;
    array.capacity = new_capacity;
    ++array.size;
  }

  template< class T >
  void clear(Array< T > & array) noexcept
  {
    delete[] array.data;
    array.data = nullptr;
    array.size = 0;
    array.capacity = 0;
  }
}

#endif
