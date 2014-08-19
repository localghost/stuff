#ifndef DS_BST_
#define DS_BST_

#include <iostream> // for LOG
#include <cassert>
#include <tuple>
#include <cstddef>
#include <utility>
#include <iterator>

#define LOG(msg) do { std::cout << msg << std::endl; } while (false)

// I is at most BidirectionalIterator
template<typename I>
class iterator_adapter
{
public:
  typedef typename std::iterator_traits<I>::value_type value_type;
  typedef typename std::iterator_traits<I>::reference reference;
  typedef typename std::iterator_traits<I>::pointer pointer;
  typedef typename std::iterator_traits<I>::iterator_category iterator_category;

  iterator_adapter() = default;
  explicit iterator_adapter(I adapted) : adaptee_{std::move(adapted)} {}

  reference operator*() const
  {
    return adaptee_.deref();
  }

  pointer operator->() const
  {
    return &adaptee_.deref();
  }

  iterator_adapter& operator++()
  {
    adaptee_.increment();
    return *this;
  }

  iterator_adapter operator++(int)
  {
    auto result = *this;
    ++*this;
    return result;
  }

  iterator_adapter& operator--()
  {
    adaptee_.decrement();
    return *this;
  }

  iterator_adapter operator--(int)
  {
    auto result = *this;
    --*this;
    return result;
  }

  friend bool operator==(const iterator_adapter& x, const iterator_adapter& y)
  {
    return x.adaptee_.state() == y.adaptee_.state();
  }

  friend bool operator!=(const iterator_adapter& x, const iterator_adapter& y)
  {
    return !(x == y);
  }

private:
  I adaptee_;
};

template<typename NodePtrType>
NodePtrType tree_min(NodePtrType n)
{
  assert(n);
  while (n->left)
    n = n->left;
  return n;
}

template<typename NodePtrType>
NodePtrType tree_max(NodePtrType n)
{
  assert(n);
  while (n->right)
    n = n->right;
  return n;
}

enum struct tree_traversal { preorder, inorder, postorder };

template<tree_traversal, typename ValueType, typename NodePtrType> class tree_iterator;

template<typename ValueType, typename NodePtrType>
class tree_iterator<tree_traversal::inorder, ValueType, NodePtrType>
{
public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ValueType value_type;
  typedef value_type& reference;
  typedef value_type* pointer;
  typedef NodePtrType state_type;
  typedef ptrdiff_t difference_type; // templetise?

  tree_iterator() = default;

  explicit tree_iterator(state_type n) : current_{n} {}

  void increment()
  {
    if (current_->right)
    {
      current_ = tree_min(current_->right);
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
      current_ = tree_max(current_->left);
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
class tree_iterator<tree_traversal::preorder, ValueType, NodePtrType>
{
public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ValueType value_type;
  typedef value_type& reference;
  typedef value_type* pointer;
  typedef NodePtrType state_type;
  typedef ptrdiff_t difference_type; // templetise?

  tree_iterator() = default;

  explicit tree_iterator(state_type n) : current_{n} {}

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
      // as long as comming from the right or from left and there is no right
      while (current_->parent && (current_->parent->right == current_ || !current_->parent->right))
        current_ = current_->parent;
      if (current_->parent && current_->parent->right)
        current_ = current_->parent->right;
    }
  }

  void decrement()
  {
    if (current_->parent)
    {
      if (current_->parent->left && current_->parent->left != current_)
      {
        current_ = tree_max(current_->parent->left);
        if (current_->left) current_ = current_->left;
      }
      else
        current_ = current_->parent;
    }
    else
    {
      current_ = tree_max(current_->left);
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



template<typename KeyType, typename MappedType>
class bst
{
public:
  typedef KeyType key_type;
  typedef MappedType mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef std::size_t size_type;

private:
  struct node;

  template<tree_traversal t>
  using titerator = tree_iterator<t, value_type, node*>;

  template<tree_traversal t>
  using iterator = iterator_adapter<titerator<t>>;

public:
  bst()
  {
    head_ = new node;
  }

  ~bst()
  {
    remove_tree(root_);
    delete head_;
  }

  // FIXME
  //  - return iterator when implemented
  //  - utilise r-value references
  bool insert(const value_type& value)
  {
    node* current = nullptr;
    node* parent = nullptr;
    std::tie(current, parent) = find_with_parent_by_key(value.first);
    if (!current)
    {
      insert_node(value, parent);
      return true;
    }
    return false;
  }
  
  // Uses Hibbard deletion algorithm (which may result in
  // unbalanced tree).
  void erase(const key_type& key)
  {
    node* current = find_by_key(key);
    if (!current) return;
    detach_node(current);
    --size_;
    delete current;
  }

  value_type& operator[](const key_type& key)
  {
    node* current = nullptr;
    node* parent = nullptr;
    std::tie(current, parent) = find_with_parent_by_key(key);
    if (!current)
      current = insert_node(std::make_pair(key, mapped_type()), parent);
    return current->value;
  }

  template<tree_traversal t>
  iterator<t> begin()
  {
    return iterator<t>{titerator<t>{root_}};
  }

  iterator<tree_traversal::inorder> begin()
  {
    constexpr auto t = tree_traversal::inorder;
    // this is optimalization, trully decoupled iterator should get root node
    return iterator<t>{titerator<t>{leftmost_}};
  }

  template<tree_traversal t>
  iterator<t> end()
  {
    return iterator<t>{titerator<t>{head_}};
  }

  iterator<tree_traversal::inorder> end()
  {
    return end<tree_traversal::inorder>();
  }

  size_type size() const
  {
    return size_;
  }

  bool empty() const
  {
    return 0 == size_;
  }

private:
  struct node
  {
    node() = default;
    explicit node(value_type value) : value{std::move(value)} {}

    value_type value;
    node* parent = nullptr;
    node* left = nullptr;
    node* right = nullptr;
  };

  const key_type& get_key(node* n)
  {
    assert(n); 
    return n->value.first; 
  }

  /// Depending on the value of `key` returns link to either left
  /// or right child.
  /// @precondition: n != nullptr 
  node*& get_link(node* n, const key_type& key)
  {
    assert(n);
    return (key < get_key(n) ? n->left : n->right);
  }

  bool is_root(node* n) const
  {
    return n == root_;
  }

  void remove_tree(node* n)
  {
    if (!n) return;
    remove_tree(n->left);
    remove_tree(n->right);
    delete n;
  }

  node* insert_node(const value_type& value, node* parent)
  {
    node* current = new node{value};
    attach_node(current, parent);
    ++size_;
    return current;
  }

  void attach_node(node* current, node* parent)
  {
    current->parent = parent;
    if (parent && parent != head_)
    {
      get_link(parent, get_key(current)) = current;
      if (parent == leftmost_ && current == parent->left)
        leftmost_ = current;
    }
    else
    {
      root_ = current;
      head_->left = root_;
      leftmost_ = current;
    }
  }

  // precodntion: current != nullptr
  void detach_node(node* current)
  {
    assert(current);
    if (!current->left && !current->right)
      detach_leaf(current);
    else if (!current->left || !current->right)
      detach_one_child(current);
    else
      detach_two_children(current);
  }

  void detach_leaf(node* n)
  {
    assert(n);
    if (!is_root(n))
    {
      if (n == leftmost_)
        leftmost_ = n->parent;
      get_link(n->parent, get_key(n)) = nullptr;
    }
    else
    {
      root_ = nullptr;
      leftmost_ = head_;
    }
  }

  void detach_one_child(node* n)
  {
    assert(n);
    node* replacement = (n->left ? n->left : n->right);
    attach_node(replacement, n->parent);
  }

  void detach_two_children(node* n)
  {
    // substitute with the max key from the left branch
    node* replacement = tree_max(n->left);
    node* replacement_parent = replacement->parent;

    // if replacement had a left child then attach it as parents
    // right; not need to check if replacement_parent != n since
    // in such case replacement_parent will be deleted in the end
    // so it doesn't matter
    if (replacement_parent && replacement_parent != n)
      replacement_parent->right = replacement->left;

    // if parent node of the one selected as a replacement is not the 
    // node that is being deleted then it needs to be adjusted as well
    if (replacement_parent != n)
    {
      replacement->left = n->left;
      replacement->left->parent = replacement;
    }

    replacement->right = n->right;
    replacement->right->parent = replacement;

    attach_node(replacement, n->parent);
  }

  node* find_by_key(const key_type& key)
  {
    node* current = root_;
    while (current && get_key(current) != key)
      current = get_link(current, key);
    return current;
  }
  
  std::pair<node*, node*> find_with_parent_by_key(const key_type& key)
  {
    node* parent = head_;
    node* current = root_;
    while (current && get_key(current) != key)
    {
      parent = current;
      current = get_link(current, key);
    }
    return std::make_pair(current, parent);
  }

  node* head_ = nullptr;
  node* root_ = nullptr;
  node* leftmost_ = nullptr;
  size_type size_ = 0;
};

#endif
