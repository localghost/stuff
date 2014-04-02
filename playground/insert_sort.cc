#include <iostream>
#include <functional>
#include <list>
#include <utility>

using std::swap;

template<typename I, typename Compare = std::less<typename I::value_type>>
void sort(I begin, I end, Compare compare = Compare())
{
  if (begin == end) return;

  for (I i = begin; i != end; ++i)
  {
    I j = i;
    I prev = j;
    --prev;
    while (j != begin && compare(*j, *prev))
    {
      swap(*j, *prev); // make use of ADL if specialized swap available
      --j;
      --prev;
    }
  }
}


int main()
{
  std::list<int> data{7, 0, -5, 9, 1234, -78, 0};
  sort(data.begin(), data.end());
//  sort(data.begin(), data.end(), std::greater<int>());
  for (auto it = data.begin(); it != data.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}
