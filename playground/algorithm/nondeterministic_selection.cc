// partition the range
// check if searched index is smaller than index of pivot or greater
// recurr appropriately
//

#include <iterator>
#include <utility>
#include <iostream>
#include <vector>

// FIXME add random pivot choice
template<typename I, typename Compare>
I select(I first, I last, size_t i, Compare compare)
{
  using std::swap;

  if (first == last) return last;

  auto pivot = first++;
  auto partition = pivot;

  while (first != last)
  {
    if (compare(*first, *pivot))
        swap(*first, *++partition);
    ++first;
  }

  if (pivot != partition)
    swap(*pivot, *partition);

  auto j = std::distance(pivot, partition);

  if (i == j) return partition;
  else if (i < j) return select(pivot, partition, i, compare);
  else if (i > j) return select(++partition, last, i - j - 1, compare);
}

int main()
{
  std::vector<int> data{4, 3, 9, 4, 5};
  auto result = select(data.begin(), data.end(), 2, std::less<int>{});
  if (data.end() != result)
    std::cout << "found: " << *result << std::endl;
  else
    std::cout << "not found" << std::endl;
}
