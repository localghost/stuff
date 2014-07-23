#include <limits>
#include <iostream>

bool does_sub_overflow(int a, int b)
{
  int max = std::numeric_limits<int>::max();
  int min = std::numeric_limits<int>::min();
  if ((b > 0) && (a < min + b)) return true;
  if ((b < 0) && (a > max + b)) return true;
  return false;
}


int main()
{
  int a = std::numeric_limits<int>::max();
  int b = -1;
  std::cout << "does " << a << " - " << b << " overflow?: " << does_sub_overflow(a, b) << std::endl;
  std::cout << a << " - " << b << " = " << a - b << std::endl;

  a = std::numeric_limits<int>::max() - 1;
  b = -1;
  std::cout << "does " << a << " - " << b << " overflow?: " << does_sub_overflow(a, b) << std::endl;
  std::cout << a << " - " << b << " = " << a - b << std::endl;
}
