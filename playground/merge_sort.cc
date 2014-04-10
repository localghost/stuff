#include <iterator>
#include <vector>
#include <iostream>

template<typename I, typename Compare>
void merge(I first, I mid, I last, Compare compare)
{
  std::vector<typename I::value_type> tmp;

  auto left = first;
  auto right = mid;
  
  while ((left != mid) || (right != last))
  {
    if ((left != mid) && (right != last))
    {
      if (!compare(*left, *right))
      {
        tmp.push_back(*right);
        ++right;
      }
      else
      {
        tmp.push_back(*left);
        ++left;
      }
    }
    else if (left != mid)
    {
      while (left != mid)
      {
        tmp.push_back(*left);
        ++left;
      }
    }
    else if (right != last)
    {
      while (right != last)
      {
        tmp.push_back(*right);
        ++right;
      }
    }
  }

  for (auto it = tmp.begin(); it != tmp.end() && first != last; ++it, ++first)
  {
    *first = *it;
  }
}

template<typename I, typename Compare>
void merge_sort(I first, I last, Compare compare)
{
  auto distance = std::distance(first, last);
  if (distance < 2) return;

  I mid = first + (distance / 2);

  merge_sort(first, mid, compare);
  merge_sort(mid, last, compare);
  merge(first, mid, last, compare);
}

int main()
{
  std::vector<int> data{6, -2, 8, 9, 15, -53, 0, 1, 8};
  for (const auto& i : data)
    std::cout << i << " ";
  std::cout << std::endl;

  merge_sort(data.begin(), data.end(), std::greater<int>());
  merge_sort(data.begin(), data.end(), std::less<int>());

  for (const auto& i : data)
    std::cout << i << " ";
  std::cout << std::endl;
}
