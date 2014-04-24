#include <vector>
#include <numeric>
#include <iostream>

int main()
{
  std::vector<int> v(100);
  std::iota(v.begin(), v.end(), 0);
  for (const auto& i : v) std::cout << i << " "; std::cout << std::endl;
  for (auto& i : v) i &= 7;
  for (const auto& i : v) std::cout << i << " "; std::cout << std::endl;
}
