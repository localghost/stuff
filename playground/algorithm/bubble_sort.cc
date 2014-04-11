#include <iostream>
#include <vector>
#include <utility>

using std::swap;

template<typename I, typename Compare>
void my_sort(I begin, I end, Compare compare)
{
    if (begin == end) return;

    I old_begin = begin;
    I old_end = end;

    while (begin != end)
    {
        begin = old_begin;
        end = old_end;
        for (--end; end != begin; --end)
        {
            I current = end;
            I prev = end;
            --prev;
            if (compare(*current, *prev))
            {
                swap(*current, *prev);
                break;
            }
        }
    }
}

int main()
{
    std::vector<int> data{7, 1, -5, 0, 4, 234, 0};
    my_sort(data.begin(), data.end(), std::less<int>());
    for (const auto& it : data)
        std::cout << it << " ";
    std::cout << std::endl;
}
