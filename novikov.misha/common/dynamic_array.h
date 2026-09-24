#ifndef COMMON_DYNAMIC_ARRAY_H_
#define COMMON_DYNAMIC_ARRAY_H_

#include <cstddef>

namespace novikov {

template <typename T>
class DynamicArray
{
public:
  DynamicArray()
    : data_(nullptr)
    , size_(0)
    , capacity_(0)
  {
  }

  DynamicArray(const DynamicArray &other)
    : data_(nullptr)
    , size_(0)
    , capacity_(0)
  {
    reserve(other.size_);
    for (std::size_t i = 0; i < other.size_; ++i) {
      data_[i] = other.data_[i];
    }
    size_ = other.size_;
  }

  DynamicArray &operator=(const DynamicArray &other)
  {
    if (this != &other) {
      clear();
      reserve(other.size_);
      for (std::size_t i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
      }
      size_ = other.size_;
    }
    return *this;
  }

  ~DynamicArray()
  {
    delete[] data_;
  }

  void pushBack(const T &value)
  {
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_] = value;
    ++size_;
  }

  void clear()
  {
    size_ = 0;
  }

  std::size_t size() const
  {
    return size_;
  }

  bool empty() const
  {
    return size_ == 0;
  }

  T &operator[](std::size_t index)
  {
    return data_[index];
  }

  const T &operator[](std::size_t index) const
  {
    return data_[index];
  }

  T *begin()
  {
    return data_;
  }

  T *end()
  {
    return data_ + size_;
  }

  const T *begin() const
  {
    return data_;
  }

  const T *end() const
  {
    return data_ + size_;
  }

private:
  void reserve(std::size_t newCapacity)
  {
    if (newCapacity <= capacity_) {
      return;
    }
    T *newData = new T[newCapacity];
    for (std::size_t i = 0; i < size_; ++i) {
      newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }

  T *data_;
  std::size_t size_;
  std::size_t capacity_;
};

}

#endif
