#include <functional>
#include <iostream>
#include <vector>
#include <iterator>

#include "utils.h"
#include "../random_sequence.h"
#include "../concepts.h"
 
// This file contains implementation of the quick-sort algorithm
// with 3-way partitioning proposed by Edsger Dijkstra. 
//
// It takes first element as the partition point so if one expects input
// to be sorted to some extent one might want to run the input data through
// std::random_shuffle() first or uncomment the part of the code that
// selects an element from the middle of the input range as the partition point.

template<BidirectionalIterator I, StrictWeakOrdering Compare>
void quicksort3(I first, I last, Compare compare)
{
  using std::swap;

  if (first == last) return;

  auto begin = first;
  auto end = last;
  auto current = begin;

  // Uncomment to select partition point from the middle of the input
  // range. Remember, however, that if I is not RandomAccessIterator
  // it will take linear time to count the distance.
  // auto middle = first + std::distance(first, last) / 2;
  // swap(*first, *middle);

  // in original implementation there is <= comparison but that
  // would require RandomAccessIterators so instead last pointing
  // to the last element it points to the one past last
  while (current != last)
  {
    if (compare(*current, *first))
      swap(*current++, *first++);
    else if (compare(*first, *current))
      // first decrement since last points to the element one past the last one
      swap(*current, *--last);
    else
      ++current;
  }

  quicksort3(begin, first, compare);
  quicksort3(last, end, compare);
}

template<BidirectionalIterator I>
void quicksort3(I first, I last)
{
  quicksort3(first, last, std::less<typename std::iterator_traits<I>::value_type>{});
}

int main()
{
    std::vector<int> data;
    random_sequence(-100, 100, 20, std::back_inserter(data));

    std::cout << "input: ";
    print_range(data.begin(), data.end());

    quicksort3(data.begin(), data.end());

    std::cout << "output: ";
    print_range(data.begin(), data.end());
}
