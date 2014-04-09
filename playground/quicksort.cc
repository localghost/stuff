#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>
#include <vector>

#include <algorithm>
#include <iterator>
#include <functional>
 
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

// I is at least ForwardIterator
// Compare is StrictWeakOrdering on I::value_type
template<typename I, typename Compare>
void naive(I begin, I end, Compare compare)
{
    if (begin == end) return;
    auto old_begin = begin;
    auto old_end = end;
    
    auto pivot = begin;
    std::cout << "pivot: " << *pivot << std::endl;

//    ++begin;

    while (begin != end)
    {
        ++begin;
        while (compare(*begin, *pivot) && (++begin != end));
        while ((begin != --end) && !compare(*end, *pivot));
        std::cout << "swapping: " << *begin << " and " << *end << std::endl;
        swap(*begin, *end);
    }
    auto partition_point = begin;
    std::cout << "partition: " << *partition_point << std::endl;
    swap(*pivot, *--partition_point);

//    I partition_point = std::partition(begin, end, [compare, pivot](const typename I::value_type& val)
//    {
//        return compare(val, pivot);
//    });
//    I pivot_point = partition_point; //std::find(begin, end, pivot);
//    std::swap(*pivot_point, *partition_point);

    naive(old_begin, partition_point, compare);    
//    partition_point = begin;
    naive(++partition_point, old_end, compare);
}

int main()
{
    std::vector<int> data = {-72, 3, 6, 45, 9, 0, -34, 2, 9};
    naive(data.begin(), data.end(), std::less<int>());
//    sort(data.begin(), data.end());
    for (const auto& i : data)
        std::cout << i << " ";
    std::cout << std::endl;
}
