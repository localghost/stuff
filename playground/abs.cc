#include <iostream>

int my_abs(int x)
{
  int y = x >> 31;
  return (x ^ y) - y;
}

int main()
{
  std::cout << "my_abs(42) = " << my_abs(42) << std::endl;
  std::cout << "my_abs(-42) = " << my_abs(-42) << std::endl;
}
