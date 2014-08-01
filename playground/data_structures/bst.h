#ifndef DS_BST_
#define DS_BST_

#include <iostream>
#include <iomanip>
#include <cassert>

class bst
{
public:
  typedef int key_type;
  typedef int value_type;

  // FIXME Return iterator when implemented.
  void insert(key_type key, value_type value)
  {
    node** current_ptr = find_by_key(key);
    node* current = *current_ptr;
    if (current)
      current->value = value;
    else
      *current_ptr = new node{key, value};
  }
  
  void erase(key_type key)
  {
    node** current_ptr = find_by_key(key);
    node* current = *current_ptr;
    if (!current) return;

    if (!current->left && !current->right)
    {
      *current_ptr = nullptr;
    }
    else if (!current->left || !current->right)
    {
      *current_ptr = (current->left ? current->left : current->right);
    }
    else
    {
      node** replacement = find_max(&current->left);
      *current_ptr = *replacement;
      if ((*replacement)->left)
        *replacement = (*replacement)->left;
      else
        *replacement = nullptr;
      (*current_ptr)->left = current->left;
      (*current_ptr)->right = current->right;
  }
    delete current;
  }

  void print()
  {
    print_subtree(root_);
    std::cout << std::endl;
  }

  ~bst()
  {
    // FIXME do it iteratively!
    //       idea: flatten the tree into a linked list using left or right
    //             pointers in the node struct
    remove_subtree(root_);
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

  void remove_subtree(node* root)
  {
    if (!root) return;
    remove_subtree(root->left);
    remove_subtree(root->right);
    delete root;
  }

  void print_subtree(node* root)
  {
    if (!root) return;
    std::cout <<  root->key << " ";
    print_subtree(root->left);
    print_subtree(root->right);
  }

  node** find_by_key(key_type key)
  {
    node** current_ptr = &root_;
    node* current = *current_ptr;
    while (current && current->key != key)
    {
      if (current->key < key)
        current_ptr = &current->right;
      else
        current_ptr = &current->left;
      current = *current_ptr;
    }
    return current_ptr;
  }

  // precondition: *start != nullptr
  node** find_max(node** start)
  {
    assert(*start);
    while ((*start)->right)
      start = &((*start)->right);
    return start;
  }

  // precondition: *start != nullptr
  node** find_min(node** start)
  {
    assert(*start);
    while ((*start)->left)
      start = &((*start)->left);
    return start;
  }

  node* root_ = nullptr;
};

#endif
