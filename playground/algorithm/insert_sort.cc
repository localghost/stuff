#include <iostream>
#include <functional>
#include <list>
#include <utility>

using std::swap;

template<typename I, typename Compare>
void sort(I first, I last, Compare compare)
{
  if (first == last) return;

  for (I i = first; i != last; ++i)
  {
    I j = i;
    I prev = j;
    --prev;
    while (j != first && compare(*j, *prev))
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
  sort(data.begin(), data.end(), std::less<int>());
//  sort(data.begin(), data.end(), std::greater<int>());
  for (auto it = data.begin(); it != data.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}
