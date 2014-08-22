#ifndef BST_ITERATOR_
#define BST_ITERATOR_

#include <cstddef>
#include <iterator>

#include "bst_utils.h"

enum struct bst_traversal { preorder, inorder, postorder };

template<bst_traversal, typename ValueType, typename NodePtrType> class bst_iterator;

template<typename ValueType, typename NodePtrType>
class bst_iterator<bst_traversal::inorder, ValueType, NodePtrType>
  : public std::iterator<std::bidirectional_iterator_tag, ValueType>
{
public:
  using typename std::iterator<std::bidirectional_iterator_tag, ValueType>::reference;
  using state_type = NodePtrType;

  bst_iterator() = default;

  explicit bst_iterator(state_type n) : current_{n} {}

  void increment()
  {
    if (current_->right)
    {
      current_ = bst_min(current_->right);
    }
    else
    {
      while (current_->parent && current_->parent->right == current_)
        current_ = current_->parent;
      current_ = current_->parent;
    }
  }

  void decrement()
  {
    if (current_->left)
    {
      current_ = bst_max(current_->left);
    }
    else
    {
      while (current_->parent && current_->parent->left == current_)
        current_ = current_->parent;
      current_ = current_->parent;
    }
  }

  reference deref() const
  {
    return current_->value;
  }

  state_type state() const
  {
    return current_;
  }

private:
  state_type current_ = nullptr;
};

template<typename ValueType, typename NodePtrType>
class bst_iterator<bst_traversal::preorder, ValueType, NodePtrType>
  : public std::iterator<std::bidirectional_iterator_tag, ValueType>
{
public:
  using typename std::iterator<std::bidirectional_iterator_tag, ValueType>::reference;
  using state_type = NodePtrType;

  bst_iterator() = default;

  explicit bst_iterator(state_type n) : current_{n} {}

  void increment()
  {
    if (current_->left)
    {
      current_ = current_->left;
    }
    else if (current_->right)
    {
      current_ = current_->right;
    }
    else
    {
      // as long as comming from the right or from the left and there is no right
      while (current_->parent && (current_->parent->right == current_ || !current_->parent->right))
        current_ = current_->parent;
      if (current_->parent && current_->parent->right)
        current_ = current_->parent->right;
    }
  }

  // FIXME This should resemble increment in post-order
  void decrement()
  {
    if (current_->parent)
    {
      if (current_->parent->left && current_->parent->left != current_)
      {
        current_ = bst_max(current_->parent->left);
        if (current_->left) current_ = current_->left;
      }
      else
        current_ = current_->parent;
    }
    else
    {
      current_ = bst_max(current_->left);
      if (current_->left) current_ = current_->left;
    }
  }

  reference deref() const
  {
    return current_->value;
  }

  state_type state() const
  {
    return current_;
  }

private:
  state_type current_ = nullptr;
};

template<typename ValueType, typename NodePtrType>
class bst_iterator<bst_traversal::postorder, ValueType, NodePtrType>
  : public std::iterator<std::bidirectional_iterator_tag, ValueType>
{
public:
  using typename std::iterator<std::bidirectional_iterator_tag, ValueType>::reference;
  using state_type = NodePtrType;

  bst_iterator() = default;

  explicit bst_iterator(state_type n) : current_{n} {}

  state_type foo(state_type n)
  {
    while (n->left || n->right)
      n = (n->left ? n->left : n->right);
    return n;
  }

  void increment()
  {
    if (current_->parent->right && current_->parent->right != current_)
      current_ = foo(current_->parent->right);
    else
      current_ = current_->parent;
  }

  void decrement()
  {
    if (current_->right)
    {
      current_ = current_->right;
    }
    else if (current_->left)
    {
      current_ = current_->left;
    }
    else
    {
      // as long as comming from the left or from the right and there is no left
      while (current_->parent && (current_->parent->left == current_ || !current_->parent->left))
        current_ = current_->parent;
      if (current_->parent && current_->parent->left)
        current_ = current_->parent->left;
    }
  }

  reference deref() const
  {
    return current_->value;
  }

  state_type state() const
  {
    return current_;
  }

private:
  state_type current_ = nullptr;
};

#endif
