#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>
#include <vector>

#include <algorithm>
#include <iterator>
#include <functional>

#include "op_counter.h"
 
using namespace std;
 
// TODO Add stable version

// I is at least BidirectionalIterator
// Compare is StrictWeakOrdering on I::value_type
template<typename I, typename Compare>
void quicksort(I first, I last, Compare compare)
{
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

template<typename I>
void print(I first, I last, const std::string& msg = std::string())
{
  std::cout << msg;
  for (auto it = first; it != last; ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}

int main()
{
    std::vector<int> data_base = {7, -72, 3, 6, 45, -72, 9, 0, -34, 2, 9};
    std::vector<op_counter<int>> data;
    for (const auto& i : data_base)
      data.emplace_back(i);
    print(data.begin(), data.end(), "IN: ");
    quicksort(data.begin(), data.end(), std::less<op_counter<int>>());
//    sort(data.begin(), data.end(), std::less<op_counter<int>>());
    print(data.begin(), data.end(), "OUT: ");
    print_results<int>();
}
