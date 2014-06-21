#include <iostream>

int fib(int n)
{
  if (n == 0) return 0;

  int first = 0;
  int next = 1;
  int result = 1;

  for (int i = 2; i <= n; ++i)
  {
    result = first + next;
    first = next;
    next = result;
  }

  return result;
}

int fib2(int n)
{
  int first = 0;
  int next = 1;
  int tmp = 0;

  for (int i = 1; i <= n; ++i)
  {
    tmp = first;
    first = next;
    next = tmp + next;
  }

  return first;
}

int main()
{
  for (int i = 0; i <= 10; ++i)
  {
    std::cout << "fib(" << i << "): " << fib(i) << std::endl;
    std::cout << "fib2(" << i << "): " << fib2(i) << std::endl;
  }
}
