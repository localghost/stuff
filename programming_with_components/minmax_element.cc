#include <utility>
#include <iostream>
#include <vector>
  
// Requirements:
//  I is ForwardIterator
//  Compare is StrictWeakOrdering on I::value_type
//  http://en.wikipedia.org/wiki/Strict_weak_order
template<typename I, typename Compare>
std::pair<I, I> minmax_element(I first, I last, Compare compare)
{
  if (first == last) return std::make_pair(last, last);

  I min = first;
  if (++first == last) return std::make_pair(min, min);
  
  I max = first;

  if (compare(*min, *max)) std::swap(min, max);

  if (++first == last) return std::make_pair(min, max);

  while (++first != last)
  {
    I potential_min = first;
    if (++first == last)
    {
      if (compare(*min, *potential_min)) min = potential_min;
      else if (!compare(*max, *potential_min)) max = potential_min;
      break;
    }

    I potential_max = first;
    if (compare(*potential_max, *potential_min))
      std::swap(potential_min, potential_max);
    if (compare(*potential_min, *min)) min = potential_min;
    if (!compare(*potential_max, *max)) max = potential_max;
  }

  return std::make_pair(min, max);
}

int main()
{
  std::vector<int> v = {3, 4, 8, 1, 9, 1, 16};

  auto result = minmax_element(v.begin(), v.end(), std::less<int>());

  std::cout << "min: " << *result.first << std::endl;
  std::cout << "max: " << *result.second << std::endl;

  return 0;
}
