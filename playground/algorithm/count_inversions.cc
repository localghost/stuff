#include <iterator>
#include <vector>
#include <iostream>

unsigned count = 0;

template<typename I, typename Compare>
unsigned count_inversions_merge(I first, I mid, I last, Compare compare)
{
  unsigned result = 0;

  std::vector<typename I::value_type> tmp;

  auto left = first;
  auto right = mid;
  
  while ((left != mid) || (right != last))
  {
    if ((left != mid) && (right != last))
    {
      if (!compare(*left, *right))
      {
        tmp.push_back(std::move(*right));
        ++right;
        result += std::distance(left, mid);
      }
      else
      {
        tmp.push_back(std::move(*left));
        ++left;
      }
    }
    else if (left != mid)
    {
      std::move(left, mid, std::back_inserter(tmp));
      left += std::distance(left, mid);
    }
    else if (right != last)
    {
      std::move(right, last, std::back_inserter(tmp));
      right += std::distance(right, last);
    }
  }

  std::move(tmp.begin(), tmp.end(), first);

  return result;
}

template<typename I, typename Compare>
unsigned count_inversions(I first, I last, Compare compare)
{
  unsigned result = 0;

  auto distance = std::distance(first, last);
  if (distance < 2) return result;

  I mid = first + (distance / 2);

  result += count_inversions(first, mid, compare);
  result += count_inversions(mid, last, compare);
  result += count_inversions_merge(first, mid, last, compare);

  return result;
}

int main()
{
  std::vector<int> data{9, 5, 6, 7, 8, 1};
  for (const auto& i : data)
    std::cout << i << " ";
  std::cout << std::endl;

  std::cout << "count = " << count_inversions(data.begin(), data.end(), std::less<int>()) << std::endl;
}
