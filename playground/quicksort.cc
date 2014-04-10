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
 
template <typename T>
void sort(T begin, T end) {
    if (begin != end) {
        T middle = std::partition (begin, end, std::bind2nd(
                    std::less<typename iterator_traits<T>::value_type>(), *begin));
        sort (begin, middle);
//        sort (max(begin + 1, middle), end);
        T new_middle = begin;
        sort (++new_middle, end);
    }
}

// I is at least BidirectionalIterator
// Compare is StrictWeakOrdering on I::value_type
template<typename I, typename Compare>
void naive(I first, I last, Compare compare)
{
    if (first == last) return;

    auto begin = first;
    auto end = last;
    
    auto pivot = first;

    while (first != last)
    {
        ++first;
        while (compare(*first, *pivot) && (++first != last));
        if (first == last) break;
        while ((first != --last) && !compare(*last, *pivot));
        swap(*first, *last);
    }
    auto partition = --first;
    swap(*pivot, *partition);

    naive(begin, partition, compare);
    naive(++partition, end, compare);
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
    std::vector<int> data_base = {-72, 3, 6, 45, -72, 9, 0, -34, 2, 9};
    std::vector<op_counter<int>> data;
    for (const auto& i : data_base)
      data.emplace_back(i);
    print(data.begin(), data.end(), "IN: ");
//    naive(data.begin(), data.end(), std::less<op_counter<int>>());
    sort(data.begin(), data.end(), std::less<op_counter<int>>());
    print(data.begin(), data.end(), "OUT: ");
    print_results<int>();
}
