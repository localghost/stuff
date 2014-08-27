#include "bst.h"

#include <limits>
#include <random>
#include <vector>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>

#define IntType typename
#define InputIterator typename
#define OutputIterator typename

std::chrono::nanoseconds time_it(std::function<void()> f)
{
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

template<bst_traversal t, typename BST>
void print(BST& tree)
{
  for (auto it = tree.template begin<t>(); it != tree.template end<t>(); ++it)
    std::cout << it->first << " ";
  std::cout << std::endl;
}

template<bst_traversal t, typename BST>
void print_reverse(BST& tree)
{
  std::reverse_iterator<decltype(tree.template begin<t>())> rbegin{tree.template end<t>()};
  std::reverse_iterator<decltype(tree.template begin<t>())> rend{tree.template begin<t>()};
  for (; rbegin != rend; ++rbegin)
    std::cout << rbegin->first << " ";
  std::cout << std::endl;
}

int main()
{
  std::vector<char> data{'F', 'G', 'I', 'H', 'B', 'A', 'D', 'E', 'C'};

  std::map<char, char, std::greater<char>> m;
  for (const int& i : data)
    m.insert(std::make_pair(i, i));
  std::cout << "MAP GREATER" << std::endl;
  for (const auto& i : m)
    std::cout << i.first << " ";
  std::cout << std::endl;

  std::cout << "\nMAP GREATER - ERASE ROOT" << std::endl;
  m.erase('F');
  for (const auto& i : m)
    std::cout << i.first << " ";
  std::cout << std::endl;

  std::cout << "\nTREE" << std::endl;
  bst<char, char> tree;
  auto duration = time_it([&]
    {
      for (const int& i : data)
        tree.insert(std::make_pair(i, i));
    });

  std::cout << "INORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<bst_traversal::inorder>(tree);
  std::cout << "decrement" << std::endl;
  print_reverse<bst_traversal::inorder>(tree);

  std::cout << "\nPREORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<bst_traversal::preorder>(tree);
  std::cout << "decrement" << std::endl;
  print_reverse<bst_traversal::preorder>(tree);

  std::cout << "\nPOSTORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<bst_traversal::postorder>(tree);
  std::cout << "decrement" << std::endl;
  print_reverse<bst_traversal::postorder>(tree);

  std::cout << "\nLEVEL" << std::endl;
  std::cout << "increment" << std::endl;
  print<bst_traversal::level>(tree);

  std::cout << "\nTREE ERASE ROOT" << std::endl;
  tree.erase('F');

  std::cout << "INORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<bst_traversal::inorder>(tree);
  std::cout << "decrement" << std::endl;
  print_reverse<bst_traversal::inorder>(tree);

  std::cout << "\nTREE GREATER" << std::endl;

  bst<char, char, std::greater<char>> tree2;
  for (const int& i : data)
    tree2.insert(std::make_pair(i, i));

  std::cout << "INORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<bst_traversal::inorder>(tree2);
  std::cout << "decrement" << std::endl;
  print_reverse<bst_traversal::inorder>(tree2);

  std::cout << "\nTREE GREATER - ERASE ROOT" << std::endl;

  tree2.erase('F');

  std::cout << "INORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<bst_traversal::inorder>(tree2);
  std::cout << "decrement" << std::endl;
  print_reverse<bst_traversal::inorder>(tree2);


  std::cout << "\nTREE COPY - COPY CTOR" << std::endl;

  auto tree_copy = tree;

  std::cout << "INORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<bst_traversal::inorder>(tree_copy);
  std::cout << "decrement" << std::endl;
  print_reverse<bst_traversal::inorder>(tree_copy);


  std::cout << "\nTREE COPY - COPY ASSIGN" << std::endl;

  decltype(tree) tree_copy2;
  tree_copy2 = tree;

  std::cout << "INORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<bst_traversal::inorder>(tree_copy2);
  std::cout << "decrement" << std::endl;
  print_reverse<bst_traversal::inorder>(tree_copy2);
}
