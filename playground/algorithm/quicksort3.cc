#include <iostream>
#include <vector>
#include <functional>

#include "utils.h"
 
using namespace std;
 
// I is at least BidirectionalIterator
// Compare is StrictWeakOrdering on I::value_type
template<typename I, typename Compare>
void quicksort3(I first, I last, Compare compare)
{
    if (first == last) return;

    auto begin = first;
    auto end = last;
    auto current = begin;

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

int main()
{
    std::vector<int> data = {7, -72, 3, 6, 7, 45, -72, 9, 0, -34, 2, 9};

    std::cout << "input: ";
    print_range(data.begin(), data.end());

    quicksort3(data.begin(), data.end(), std::less<int>());

    std::cout << "output: ";
    print_range(data.begin(), data.end());
}
