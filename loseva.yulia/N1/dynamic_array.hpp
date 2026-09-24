#ifndef LOSEVA_DYNAMIC_ARRAY_HPP
#define LOSEVA_DYNAMIC_ARRAY_HPP

#include <cseddef>

namespace loseva {

template <typename T>
struct DynamicArray {
  T* data;
  std::size_t capacity_;
  std::size_t size_;
}

}

#endif
