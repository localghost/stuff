#include "rbt.h"

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
//  std::vector<int> data{6, 7, 3, 4, 0, 9, 1, 2, 5, 8};
  std::vector<int> data{'s', 'e', 'a', 'r', 'c', 'h', 'x', 'm', 'p', 'l'};
//  std::iota(data.begin(), data.end(), 0);
//  std::random_shuffle(data.begin(), data.end());
//  std::vector<int> data;
//  random_sequence(std::numeric_limits<int>::min(),
//                  std::numeric_limits<int>::max(),
//                  10,
//                  std::back_inserter(data));
  rbt tree;
  auto duration = time_it([&]
    {
      for (const int& i : data)
      {
        tree.insert(i, i);
      }
    });
//  std::cout << "bst: " << duration.count() << std::endl;
  tree.print();
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
//  tree.print();
}
