#include <iostream>

bool is_odd(int n)
{
  return (n & 1);
}

int main()
{
  std::cout << "12: " << is_odd(12) << std::endl;
  std::cout << "13: " << is_odd(13) << std::endl;
  std::cout << "-12: " << is_odd(-12) << std::endl;
  std::cout << "-13: " << is_odd(-13) << std::endl;
}
