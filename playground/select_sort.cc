#include <iostream>
#include <utility>
#include <list>

using std::swap;

template<typename I, typename Compare = std::less<typename I::value_type>>
void sort(I begin, I end, Compare compare = Compare())
{
  for (; begin != end; ++begin)
  {
    I min = begin;
    I next = begin;
    ++next;
    for (; next != end; ++next)
    {
      if (compare(*next, *min))
        min = next;
    }
    // is occassional swap of the same element more expensive than compare on each iteration?
    if (min != begin)
      swap(*min, *begin);
  }
}

int main()
{
  std::list<int> data{7, -8, 0, 5, 6, -2, 3, 167, -45, 0};
  sort(data.begin(), data.end());
//  sort(data.begin(), data.end(), std::greater<int>());
  for (auto it = data.begin(); it != data.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}
