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

template<typename I, typename Compare>
void quicksort_1(I first, I last, Compare compare)
{
  using std::swap;
  if (first == last) return;

  auto begin = first;
  auto end = last;
  auto pivot = *(first + (std::distance(first, last) / 2));

  while (first != last)
  {
    while (first != last && compare(*first, pivot))
      ++first;

    while (first != last && compare(pivot, *--last));

    if (first != last)
      swap(*first, *last);
  }

  quicksort_1(begin, first, compare);
  quicksort_1(++first, end, compare);
}

template<typename I, typename Compare>
void quicksort_2(I first, I last, Compare compare)
{
  using std::swap;
  if (first == last) return;

  auto begin = first;
  auto end = last;
  auto pivot = first++;
  auto partition = pivot;

  while (first != last)
  {
    if (compare(*first, *pivot))
        swap(*first, *++partition);
    ++first;
  }

  if (pivot != partition)
    swap(*pivot, *partition);

  quicksort_2(begin, partition, compare);
  quicksort_2(++partition, end, compare);
}

struct tag_1 {};
struct tag_2 {};

int main()
{
  std::vector<int> data_base = {7, -72, 3, 6, 45, -72, 9, 0, -34, 2, 9};
  std::vector<op_counter<int>> data;
  std::vector<op_counter<int, tag_1>> data_1;
  std::vector<op_counter<int, tag_2>> data_2;
  for (const auto& i : data_base)
  {
    data.emplace_back(i);
    data_1.emplace_back(i);
    data_2.emplace_back(i);
  }

  std::cout << "input: ";
  print_range(data.begin(), data.end());

  quicksort(data.begin(), data.end(), std::less<op_counter<int>>());
  quicksort_1(data_1.begin(), data_1.end(), std::less<op_counter<int, tag_1>>());
  quicksort_2(data_2.begin(), data_2.end(), std::less<op_counter<int, tag_2>>());
  //    sort(data.begin(), data.end(), std::less<op_counter<int>>());

  std::cout << "output: ";
  print_range(data.begin(), data.end());

  print_results<int>();
  print_results<int, tag_1>();
  print_results<int, tag_2>();
}
