#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>
#include <vector>

// I is at least ForwardIterator
// Compare is StrictWeakOrdering on I::value_type
template<typename I, typename Compare>
void naive(I begin, I end, Compare compare)
{
    if (begin == end) return;
    
    auto pivot = *begin;

    I partition_point = std::partition(begin, end, [compare, pivot](const typename I::value_type& val)
    {
        return compare(val, pivot);
    });
    I pivot_point = std::find(begin, end, pivot);
    std::swap(*pivot_point, *partition_point);

    naive(begin, partition_point, compare);    
    naive(++partition_point, end, compare);
}

int main()
{
    std::vector<int> data = {-72, 3, 6, 45, 9, 0, -34, 2, 9};
    naive(data.begin(), data.end(), std::greater<int>{});
    for (const auto& i : data)
        std::cout << i << " ";
    std::cout << std::endl;
}
