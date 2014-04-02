#include <iostream>
#include <functional>
#include <vector>

template<typename I, typename T, typename Compare = std::less<T>>
I bsearch(I begin, I end, const T& val, Compare compare = Compare())
{
    if (begin == end) return end;

    I not_found = end;

    I middle = begin;
    while (begin != end)
    {
        middle = begin + std::distance(begin, end) / 2;
        if (compare(*middle, val))
        {
            begin = ++middle;
        }
        else
        {
            end = middle;
        }
    }
    if (*middle == val)
        return middle;
    return not_found;
}

int main()
{
    std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 8, 9};
//    std::vector<int> data{9, 8, 8, 7, 6, 5};
    auto it = bsearch(data.begin(), data.end(), 10);
//    auto it = bsearch(data.begin(), data.end(), 8, std::greater<int>());
    if (it != data.end())
        std::cout << "found: " << *it << std::endl;
}
