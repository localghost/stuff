#ifndef BST_ITERATOR_
#define BST_ITERATOR_

#include <cstddef>
#include <iterator>
#include <deque>

#include "bst_utils.h"

enum struct bst_traversal { preorder, inorder, postorder, level };

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

  void decrement()
  {
    // If coming from the right, visit left; if not, visit parent
    if (current_->parent && current_->parent->left && current_->parent->left != current_)
    {
      current_ = bst_max(current_->parent->left);
      if (current_->left) current_ = current_->left;
    }
    else if (current_->parent)
    {
      current_ = current_->parent;
    }
    else
    {
      // FIXME This branch is only to accomodate to the fact that head is used as
      //       end marker; what's more it introduces infinite loop (but that should
      //       not be a real issue since decrementing begin() is undefined)
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

  void increment()
  {
    if (current_->parent->right && current_->parent->right != current_)
    {
      current_ = current_->parent->right;
      while (current_->left || current_->right)
        current_ = (current_->left ? current_->left : current_->right);
    }
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

template<typename ValueType, typename NodePtrType>
class bst_iterator<bst_traversal::level, ValueType, NodePtrType>
  : public std::iterator<std::forward_iterator_tag, ValueType>
{
public:
  using typename std::iterator<std::forward_iterator_tag, ValueType>::reference;
  using state_type = std::deque<NodePtrType>;

  bst_iterator() = default;

  explicit bst_iterator(typename state_type::value_type n)
  {
    if (n) nodes_.push_back(std::move(n));
  }

  void increment()
  {
    const auto& current = nodes_.front();
    if (current->left) nodes_.push_back(current->left);
    if (current->right) nodes_.push_back(current->right);
    nodes_.pop_front();
  }

  reference deref() const
  {
    return nodes_.front()->value;
  }

  const state_type& state() const
  {
    return nodes_;
  }

private:
  state_type nodes_;
};

#endif
