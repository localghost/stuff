#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>
#include <vector>

#include <algorithm>
#include <iterator>
#include <functional>

#include "op_counter.h"
#include "utils.h"
 
// TODO Add stable version

// I is at least BidirectionalIterator
// Compare is StrictWeakOrdering on I::value_type
template<typename I, typename Compare>
void quicksort(I first, I last, Compare compare)
{
  using std::swap;
  if (first == last) return;

  auto begin = first;
  auto end = last;

  auto pivot = first + (std::distance(first, last) / 2);
  swap(*pivot, *first);
  pivot = first;
  ++first;

  while (first != last)
  {
    while (!compare(*pivot, *first) && (++first != last));
    if (first == last) break;
    while ((first != --last) && compare(*pivot, *last));
    swap(*first, *last);
  }
  auto partition = --first;
  swap(*pivot, *partition);

  quicksort(begin, partition, compare);
  quicksort(++partition, end, compare);
}

int main()
{
  std::vector<int> data_base = {7, -72, 3, 6, 45, -72, 9, 0, -34, 2, 9};
  std::vector<op_counter<int>> data;
  for (const auto& i : data_base)
    data.emplace_back(i);

  std::cout << "input: ";
  print_range(data.begin(), data.end());

  quicksort(data.begin(), data.end(), std::less<op_counter<int>>());
  //    sort(data.begin(), data.end(), std::less<op_counter<int>>());

  std::cout << "output: ";
  print_range(data.begin(), data.end());

  print_results<int>();
}
