#include "bst.h"

#include <limits>
#include <random>
#include <vector>
#include <chrono>
#include <functional>

#define IntType typename
#define InputIterator typename
#define OutputIterator typename

// ValueType(OutputIterator) == T
template<IntType T, OutputIterator O>
O random_sequence(T from, T to, unsigned how_many, O output)
{
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<T> dist{from, to};
  for (unsigned i = 0; i < how_many; ++i)
    *output++ = dist(gen);
  return output;
}

std::chrono::nanoseconds time_it(std::function<void()> f)
{
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

int main()
{
  std::vector<char> data{'F', 'G', 'I', 'H', 'B', 'A', 'D', 'E', 'C'};
//  std::iota(data.begin(), data.end(), 0);
//  std::random_shuffle(data.begin(), data.end());
//  std::vector<int> data;
//  random_sequence(std::numeric_limits<int>::min(),
//                  std::numeric_limits<int>::max(),
//                  10,
//                  std::back_inserter(data));
  bst<char, char> tree;
  auto duration = time_it([&]
    {
      for (const int& i : data)
        tree.insert(std::make_pair(i, i));
    });
  for (auto& val : tree)
    std::cout << val.first << std::endl;
  for (auto it = --tree.end(); it != tree.begin(); --it)
    std::cout << it->first << std::endl;

//  auto it = ++++++tree.begin();
//  std::cout << "--: " << (--it)->first << std::endl;
//  for (auto it = --(tree.end()); it != tree.begin(); --it)
//    std::cout << it->first << std::endl;
//  std::cout << "bst: " << duration.count() << std::endl;
//  tree.erase(6);
//  tree.erase(5);
//  tree.erase(3);
//  tree.erase(7);
//  tree.erase(2);
//  tree.erase(1);
//  tree.erase(8);
//  tree.erase(4);
//  tree.erase(0);
//  tree.erase(9);
//
//  for (auto& val : tree)
//    std::cout << val.first << std::endl;
//  tree.print();


//  random_sequence(std::numeric_limits<int>::min(),
//                  std::numeric_limits<int>::max(),
//                  100,
//                  std::back_inserter(data));
//
//  for (const int& i : data)
//    tree.insert(std::make_pair(i, i));
////  tree.print();
//  for (auto& val : tree)
//    std::cout << val.first << std::endl;
//  bst<int, int> tree2;
//  tree2.insert(std::make_pair(2, 2));
//  tree2.insert(std::make_pair(1, 1));
//  tree2.insert(std::make_pair(3, 3));
  std::cout << "\nPREORDER\n" << std::endl;
  std::cout << "increment" << std::endl;
  for (auto it = tree.begin<tree_traversal::preorder>(); it != tree.end<tree_traversal::preorder>(); ++it)
    std::cout << it->first << std::endl;
  std::cout << "decrement" << std::endl;
  for (auto it = --tree.end<tree_traversal::preorder>(); it != tree.begin<tree_traversal::preorder>(); --it)
    std::cout << it->first << std::endl;

//  auto res = tree.insert(std::make_pair('Z', 'Z'));
//  std::cout << "inserted: " << res.second << ", it: " << res.first->first << std::endl;
//  for (auto& val : tree)
//    std::cout << val.first << std::endl;

  std::cout << "\nPOSTORDER\n" << std::endl;
  std::cout << "increment" << std::endl;
  for (auto it = tree.begin<tree_traversal::postorder>(); it != tree.end<tree_traversal::postorder>(); ++it)
    std::cout << it->first << std::endl;
  std::cout << "decrement" << std::endl;
  for (auto it = --tree.end<tree_traversal::postorder>(); it != tree.begin<tree_traversal::postorder>(); --it)
    std::cout << it->first << std::endl;
}
