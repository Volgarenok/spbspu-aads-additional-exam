#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <type_traits>

namespace pozdnyakov {
  template< class Key, class Value >
  struct HashNode {
    Key key;
    Value value;
    HashNode * next;
  };

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  struct HashTable {
    HashNode< Key, Value > ** buckets;
    std::size_t bucket_count;
    std::size_t size;
  };

  namespace detail {
    template< class Key, class Value, class Hash, class Equal >
    std::size_t getBucket(const HashTable< Key, Value, Hash, Equal > & table, const Key & key)
    {
      return Hash()(key) % table.bucket_count;
    }

    template< class Key, class Value, class Hash, class Equal >
    HashNode< Key, Value > * findNode(const HashTable< Key, Value, Hash, Equal > & table, const Key & key)
    {
      if (table.bucket_count == 0) {
        return nullptr;
      }
      HashNode< Key, Value > * node = table.buckets[getBucket(table, key)];
      while (node != nullptr && !Equal()(node->key, key)) {
        node = node->next;
      }
      return node;
    }

    template< class Key, class Value, class Hash, class Equal >
    void rehash(HashTable< Key, Value, Hash, Equal > & table, std::size_t bucket_count)
    {
      using NodePointer = typename std::add_pointer< HashNode< Key, Value > >::type;
      NodePointer * const buckets = new NodePointer[bucket_count]{};
      for (std::size_t i = 0; i < table.bucket_count; ++i) {
        HashNode< Key, Value > * node = table.buckets[i];
        while (node != nullptr) {
          HashNode< Key, Value > * const next = node->next;
          const std::size_t index = Hash()(node->key) % bucket_count;
          node->next = buckets[index];
          buckets[index] = node;
          node = next;
        }
      }
      delete[] table.buckets;
      table.buckets = buckets;
      table.bucket_count = bucket_count;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value * find(HashTable< Key, Value, Hash, Equal > & table, const Key & key)
  {
    HashNode< Key, Value > * const node = detail::findNode(table, key);
    return node == nullptr ? nullptr : &node->value;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value * find(const HashTable< Key, Value, Hash, Equal > & table, const Key & key)
  {
    const HashNode< Key, Value > * const node = detail::findNode(table, key);
    return node == nullptr ? nullptr : &node->value;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value & emplace(HashTable< Key, Value, Hash, Equal > & table, const Key & key, const Value & value)
  {
    HashNode< Key, Value > * const found = detail::findNode(table, key);
    if (found != nullptr) {
      return found->value;
    }
    constexpr std::size_t initial_bucket_count = 16;
    constexpr std::size_t growth_factor = 2;
    if (table.bucket_count == 0) {
      detail::rehash(table, initial_bucket_count);
    } else if (table.size >= table.bucket_count) {
      detail::rehash(table, table.bucket_count * growth_factor);
    }
    const std::size_t index = detail::getBucket(table, key);
    HashNode< Key, Value > * const node = new HashNode< Key, Value >{key, value, table.buckets[index]};
    table.buckets[index] = node;
    ++table.size;
    return node->value;
  }

  template< class Key, class Value, class Hash, class Equal >
  void clear(HashTable< Key, Value, Hash, Equal > & table) noexcept
  {
    for (std::size_t i = 0; i < table.bucket_count; ++i) {
      HashNode< Key, Value > * node = table.buckets[i];
      while (node != nullptr) {
        HashNode< Key, Value > * const next = node->next;
        delete node;
        node = next;
      }
    }
    delete[] table.buckets;
    table.buckets = nullptr;
    table.bucket_count = 0;
    table.size = 0;
  }
}

#endif
