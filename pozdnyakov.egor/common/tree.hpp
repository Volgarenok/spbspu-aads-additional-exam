#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <functional>

namespace pozdnyakov {
  template< class Key, class Value >
  struct TreeNode {
    Key key;
    Value value;
    TreeNode * left;
    TreeNode * right;
  };

  template< class Key, class Value, class Compare = std::less< Key > >
  struct Tree {
    TreeNode< Key, Value > * root;
    std::size_t size;
  };

  namespace detail {
    template< class Key, class Value >
    void destroyNodes(TreeNode< Key, Value > * node) noexcept
    {
      while (node != nullptr) {
        destroyNodes(node->left);
        TreeNode< Key, Value > * const right = node->right;
        delete node;
        node = right;
      }
    }
  }

  template< class Key, class Value, class Compare >
  bool insert(Tree< Key, Value, Compare > & tree, const Key & key, const Value & value)
  {
    TreeNode< Key, Value > ** place = &tree.root;
    while (*place != nullptr) {
      if (Compare()(key, (*place)->key)) {
        place = &(*place)->left;
      } else if (Compare()((*place)->key, key)) {
        place = &(*place)->right;
      } else {
        return false;
      }
    }
    *place = new TreeNode< Key, Value >{key, value, nullptr, nullptr};
    ++tree.size;
    return true;
  }

  template< class Key, class Value, class Compare >
  const Value * find(const Tree< Key, Value, Compare > & tree, const Key & key)
  {
    const TreeNode< Key, Value > * node = tree.root;
    while (node != nullptr) {
      if (Compare()(key, node->key)) {
        node = node->left;
      } else if (Compare()(node->key, key)) {
        node = node->right;
      } else {
        return &node->value;
      }
    }
    return nullptr;
  }

  template< class Key, class Value, class Compare >
  void clear(Tree< Key, Value, Compare > & tree) noexcept
  {
    detail::destroyNodes(tree.root);
    tree.root = nullptr;
    tree.size = 0;
  }
}

#endif
