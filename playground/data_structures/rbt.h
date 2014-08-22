#ifndef DS_RBT_
#define DS_RBT_

#include <iostream>
#include <iomanip>
#include <cassert>
#include <tuple>
#include <deque> // for print

class rbt
{
  struct node;
public:
  typedef int key_type;
  typedef int value_type;

  // FIXME Return iterator when implemented.
  void insert(const key_type& key, const value_type& value)
  {
    node* current;
    node* parent;
    direction dir;
    std::tie(current, parent, dir) = find_by_key(key);
    if (!current)
      insert_node(key, value, parent, dir);
    else
      current->value = value;
  }
  
  // Uses Hibbard deletion algorithm (which may result in
  // unbalanced tree).
  void erase(key_type key)
  {
    node* current;
    node* parent;
    direction dir;
    std::tie(current, parent, dir) = find_by_key(key);
    if (!current) return;
    detach_node(current, parent, dir);
    delete current;
  }

  value_type& operator[](const key_type& key)
  {
    node* current;
    node* parent;
    direction dir;
    std::tie(current, parent, dir) = find_by_key(key);
    if (!current)
      current = insert_node(key, value_type(), parent, dir);
    return current->value;
  }

  int node_to_str(node* n)
  {
    return (n ? n->key : -1);
  }

  void print()
  {
    if (!root_) return;
    node null_node{-1, -1, nullptr};
    std::deque<std::pair<node*, unsigned>> queue;
    queue.push_back(std::make_pair(root_, 0));
    unsigned current_level = 0;
    while (!queue.empty())
    {
      auto n = queue.front();
      queue.pop_front();
      if (n.second > current_level)
      { 
        std::cout << std::endl;
        current_level = n.second;
      }
      std::cout << n.first->key << "[" << (n.first->c == color::black ? "b" : "r") << ", " << node_to_str(n.first->parent) << ", " << node_to_str(n.first->left) << ", " << node_to_str(n.first->right) << "]"<< " ";
//      std::cout << n.first->key << (n.first->c == color::black ? "b" : "r") << " ";
      if (n.first->right && n.first->key == -1) std::cout << "-1 has right child" << std::endl;
      if (n.first->left && n.first->key == -1) std::cout << "-1 has left child" << std::endl;
      if (n.first->left)
        queue.push_back(std::make_pair(n.first->left, current_level+1));
      else if (n.first->key != -1)
        queue.push_back(std::make_pair(&null_node, current_level+1));
      if (n.first->right)
        queue.push_back(std::make_pair(n.first->right, current_level+1));
      else if (n.first->key != -1)
        queue.push_back(std::make_pair(&null_node, current_level+1));
    }
    std::cout << std::endl;
  }

  ~rbt()
  {
    remove_tree(root_);
  }

private:
  enum struct direction : bool { left, right };

  enum struct color : bool { red, black };

  struct node
  {
    node() = default;
    node(key_type key, value_type value, node* parent)
      : key{key},
        value{value},
        c{parent ? color::red : color::black},
        parent{parent}
    { } 

    key_type key;
    value_type value;
    color c = color::black;
    node* parent = nullptr;
    node* left = nullptr;
    node* right = nullptr;
  };

  void remove_tree(node* root)
  {
    if (!root) return;
    remove_tree(root->left);
    remove_tree(root->right);
    delete root;
  }

  void print_subtree(node* root)
  {
    if (!root) return;
    std::cout <<  root->key << " ";
    print_subtree(root->left);
    print_subtree(root->right);
  }

  node* insert_node(const key_type& key, const value_type& value, node* parent, direction dir)
  {
    node* current = new node{key, value, parent};
    attach_node(current, parent, dir);
    return current;
  }

  void attach_node(node* current, node* parent, direction dir)
  {
    if (parent)
    {
      if (direction::left == dir) 
        parent->left = current;
      else
        parent->right = current;
      rebalance(current);
    }
    else
      root_ = current;
  }

  // precodntion: current != nullptr
  void detach_node(node* current, node* parent, direction dir)
  {
    if (!current->left && !current->right)
    {
      // remove leaf node
      attach_node(nullptr, parent, dir);
    }
    else if (!current->left || !current->right)
    {
      // remove node with 1 child
      node* replacement = (current->left ? current->left : current->right);
      attach_node(replacement, parent, dir);
    }
    else
    {
      // remove node with 2 children
      node* replacement;
      node* replacement_parent;
      // substitute with the max key from the left branch
      std::tie(replacement, replacement_parent) = find_max(current->left);
      attach_node(replacement, parent, dir);
      if (replacement->left && replacement_parent)
        replacement_parent->right = replacement->left;
      else if (replacement_parent)
        replacement_parent->right = nullptr;

      if (replacement_parent)
        replacement->left = current->left;
      else // it means the replacement_parent is the node that is being deleted
        replacement->left = nullptr;
      replacement->right = current->right;
    }
  }

  std::tuple<node*, node*, direction> find_by_key(key_type key)
  {
    node* current = root_;
    node* parent = nullptr;
    direction dir = direction::left;
    while (current && current->key != key)
    {
      parent = current;
      if (current->key < key)
      {
        dir = direction::right;
        current = current->right;
      }
      else
      {
        dir = direction::left;
        current = current->left;
      }
    }
    return std::make_tuple(current, parent, dir);
  }

  // precondition: start != nullptr
  std::tuple<node*, node*> find_max(node* start)
  {
    assert(start);
    node* parent = nullptr;
    while (start->right)
    {
      parent = start;
      start = start->right;
    }
    return std::make_tuple(start, parent);
  }

  node* rotate_left(node* current)
  {
    std::cout << "rotate-left: " << current->parent->key << " <-> " << current->key << std::endl;
    assert(current->c == color::red);
    node* parent = current->parent;
    assert(parent);
    auto pparent = parent->parent;
    auto left = current->left;
    current->left = parent;
    parent->right = left;
    if (parent->right)
    parent->right->parent = parent;
    current->c = parent->c;
    parent->c = color::red;

    current->parent = pparent;
    parent->parent = current;
    if (!current->parent) root_ = current;
//    if (!current->parent)
//      set_as_root(current);

    if (pparent) {
    if (pparent->left == parent)
      pparent->left = current;
    else
      pparent->right = current;
    }
    return parent;
  }

  void set_as_root(node* current)
  {
    root_ = current;
    current->c = color::black;
  }

  void swap_node_links(node* x, node* y)
  {
    std::swap(x->parent, y->parent);
    std::swap(x->left, y->left);
    std::swap(x->right, y->right);
  }

  node* rotate_right(node* current)
  {
    std::cout << "rotate-right: " << current->parent->key << " <-> " << current->key << std::endl;
    assert(current->c == color::red);
    node* parent = current->parent;
    assert(parent);
    if (parent)
    {
      auto pparent = parent->parent;
      auto right = current->right;
      current->c = parent->c;
      parent->c = color::red;
      current->right = parent;
      parent->left = right;
      if (parent->left)
      parent->left->parent = parent;
      current->parent = parent->parent;
      if (!current->parent) root_ = current;
//        set_as_root(current);

      parent->parent = current;
      if (pparent) {
      if (pparent->left == parent)
        pparent->left = current;
      else
        pparent->right = current;
      }
    }
    return parent;
  }

  void flip_colors(node* left, node* right)
  {
    std::cout << "flip: " << left->key << " && " << right->key << std::endl;
    assert(left->c == color::red);
    assert(right->c == color::red);
    assert(left->parent->c == color::black);
    // root_ is always black
    if (left->parent->parent)
      left->parent->c = color::red;
    left->c = color::black;
    right->c = color::black;
  }

  void rebalance(node* current)
  {
    bool must_rebalance = false;

    if (is_red(current) && current->parent && current->parent->right == current && (!current->parent->left || !is_red(current->parent->left)))
    {
      current = rotate_left(current);
      must_rebalance = true;
    }

    if (is_red(current) && current->parent && is_red(current->parent))
    {
      rotate_right(current->parent);
      must_rebalance = true;     
    }

    if (current->parent && current->parent->left && current->parent->right && is_red(current->parent->left) && is_red(current->parent->right))
    {
      flip_colors(current->parent->left, current->parent->right);
      current = current->parent;
      must_rebalance = true;
    }

    if (must_rebalance) rebalance(current);
  }

  bool is_red(node* current)
  {
    return current->c == color::red;
  }

  node* root_ = nullptr;
};

#endif
