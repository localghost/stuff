#ifndef BST_
#define BST_

#include <cassert>
#include <tuple>
#include <cstddef>
#include <utility>
#include <memory>

#include "bst_utils.h"
#include "bst_iterator.h"
#include "iterator_adapter.h"

template<typename KeyType,
         typename MappedType,
         typename ComparatorType = std::less<KeyType>,
         typename AllocatorType = std::allocator<std::pair<const KeyType, MappedType>>>
class bst
{
public:
  typedef KeyType key_type;
  typedef MappedType mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef ComparatorType key_compare;
//  typedef ... value_compare;
  typedef AllocatorType allocator_type;

  typedef std::size_t size_type;

private:
  struct node;

  template<bst_traversal t>
  using titerator = bst_iterator<t, value_type, node*>;

  template<bst_traversal t>
  using iterator = iterator_adapter<titerator<t>>;

  using node_allocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<node>;

  using node_traits = std::allocator_traits<node_allocator>;

public:
  bst()
  {
    head_ = create_node();
  }

  bst(const bst& other)
  {
    head_ = create_node();
    head_->left = clone_tree(other.root_, head_);
    root_ = head_->left;
    leftmost_ = bst_min(root_);
    size_ = other.size_;
  }

  bst(bst&& other)
    : head_{other.head_},
      root_{other.root_},
      leftmost_{other.leftmost_},
      size_{other.size_},
      allocator_{other.allocator_}
  {
    other.head_ = nullptr;
    other.root_ = nullptr;
    other.leftmost_ = nullptr;
    other.size_ = 0;
  }

  ~bst()
  {
    if (head_) remove_tree(head_);
  }

  bst& operator=(const bst& other)
  {
    if (root_) remove_tree(root_);
    head_->left = clone_tree(other.root_, head_);
    root_ = head_->left;
    leftmost_ = bst_min(root_);
    size_ = other.size_;
  }

  bst& operator=(bst&& other)
  {
    if (head_) remove_tree(head_);
    head_ = nullptr;
    root_ = nullptr;
    leftmost_ = nullptr;
    size_ = 0;
    std::swap(*this, other);
  }

  // FIXME utilise r-value references
  std::pair<iterator<bst_traversal::inorder>, bool>
  insert(const value_type& value)
  {
    node* current = nullptr;
    node* parent = nullptr;
    std::tie(current, parent) = find_with_parent_by_key(value.first);
    if (!current)
    {
      current = insert_node(value, parent);
      return std::make_pair(make_iterator<bst_traversal::inorder>(current), true);
    }
    return std::make_pair(make_iterator<bst_traversal::inorder>(current), false);
  }
  
  // Uses Hibbard deletion algorithm (which may result in unbalanced tree).
  // see http://algs4.cs.princeton.edu/32bst/
  size_type erase(const key_type& key)
  {
    node* current = find_by_key(key);
    if (!current) return 0;
    detach_node(current);
    --size_;
    destroy_node(current);
    return 1;
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

  template<bst_traversal t>
  iterator<t> begin()
  {
    if (bst_traversal::preorder == t || bst_traversal::level == t)
      return make_iterator<t>(root_);
    return make_iterator<t>(leftmost_);
  }

  iterator<bst_traversal::inorder> begin()
  {
    return begin<bst_traversal::inorder>();
  }

  template<bst_traversal t>
  iterator<t> end()
  {
    if (bst_traversal::level == t)
      return iterator<t>{titerator<t>{}};
    return make_iterator<t>(head_);
  }

  iterator<bst_traversal::inorder> end()
  {
    return end<bst_traversal::inorder>();
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

  // Depending on the value of `key` returns link to either left or right child.
  // precondition: n != nullptr 
  node*& get_link(node* n, const key_type& key)
  {
    assert(n);
    return (key_compare{}(key, get_key(n)) ? n->left : n->right);
  }

  bool is_root(node* n) const
  {
    return n == root_;
  }

  node* clone_tree(node* n, node* p)
  {
    node* result = create_node(n->value);
    result->parent = p;
    if (n->left) result->left = clone_tree(n->left, result);
    if (n->right) result->right = clone_tree(n->right, result);
    return result;
  }

  // precondition: c != nullptr
  void remove_tree(node* n)
  {
    assert(n);
    if (n->left) remove_tree(n->left);
    if (n->right) remove_tree(n->right);
    destroy_node(n);
  }

  node* insert_node(const value_type& value, node* parent)
  {
    node* current = create_node(value);
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
      if (!leftmost_ || key_compare{}(get_key(current), get_key(leftmost_)))
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

  // Removes the node.
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

  // Replaces removed node with its child.
  void detach_one_child(node* n)
  {
    assert(n);
    node* replacement = (n->left ? n->left : n->right);
    attach_node(replacement, n->parent);
  }

  void detach_two_children(node* n)
  {
    assert(n);

    // substitute with the max key from the left branch
    node* replacement = bst_max(n->left);
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

  node_allocator& get_allocator()
  {
    return allocator_;
  }

  template<typename ...Args>
  node* create_node(Args&&... args)
  {
    node* result = node_traits::allocate(get_allocator(), 1);
    try
    {
      node_traits::construct(get_allocator(), result, std::forward<Args>(args)...);
    }
    catch (...)
    {
      node_traits::deallocate(get_allocator(), result, 1);
      throw;
    }
    return result;
  }

  // precondition: n != nullptr
  void destroy_node(node* n)
  {
    assert(n);
    node_traits::destroy(get_allocator(), n);
    node_traits::deallocate(get_allocator(), n, 1);
  }

  template<bst_traversal t>
  iterator<t> make_iterator(node* n)
  {
    return iterator<t>{titerator<t>{n}};
  }

  node* head_ = nullptr;
  node* root_ = nullptr;
  // for constant time begin() (for inorder and postorder)
  node* leftmost_ = nullptr;
  size_type size_ = 0;
  node_allocator allocator_;
};

#endif
