#include "bst.h"

#include <limits>
#include <random>
#include <vector>
#include <chrono>
#include <functional>

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

template<tree_traversal t, typename BST>
void print(BST& tree)
{
  for (auto it = tree.template begin<t>(); it != tree.template end<t>(); ++it)
    std::cout << it->first << " ";
  std::cout << std::endl;
}

template<tree_traversal t, typename BST>
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

  bst<char, char> tree;
  auto duration = time_it([&]
    {
      for (const int& i : data)
        tree.insert(std::make_pair(i, i));
    });

  std::cout << "INORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<tree_traversal::inorder>(tree);
  std::cout << "decrement" << std::endl;
  print_reverse<tree_traversal::inorder>(tree);

  std::cout << "\nPREORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<tree_traversal::preorder>(tree);
  std::cout << "decrement" << std::endl;
  print_reverse<tree_traversal::preorder>(tree);


  std::cout << "\nPOSTORDER" << std::endl;
  std::cout << "increment" << std::endl;
  print<tree_traversal::postorder>(tree);
  std::cout << "decrement" << std::endl;
  print_reverse<tree_traversal::postorder>(tree);
}
