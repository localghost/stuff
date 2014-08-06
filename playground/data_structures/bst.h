#ifndef DS_BST_
#define DS_BST_

#include <iostream>
#include <iomanip>
#include <cassert>
#include <tuple>
#include <deque> // for print
#include <cstddef>

class bst
{
public:
  typedef int key_type;
  typedef int value_type;
  typedef std::size_t size_type

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

  void print()
  {
    if (!root_) return;
    node null_node{-1, -1};
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
      std::cout << n.first->key << " ";
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

  ~bst()
  {
    remove_tree(root_);
  }

private:
  struct node
  {
    node() = default;
    node(key_type key, value_type value) : key{key}, value{value} {}

    key_type key;
    value_type value;
    node* left = nullptr;
    node* right = nullptr;
  };

  enum struct direction : bool { left, right };

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
    node* current = new node{key, value};
    attach_node(current, parent, dir);
    return current;
  }

  void attach_node(node* current, node* parent, direction dir)
  {
    if (parent)
    {
      if (direction::left == dir) parent->left = current;
      else parent->right = current;
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

  node* root_ = nullptr;
  size_type size_ = 0;
};

#endif
