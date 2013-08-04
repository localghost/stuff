#include <cstdio>

unsigned gcd(unsigned m, unsigned n)
{
  unsigned r = m % n;
  while (0 != r)
  {
    m = n;
    n = r;
    r = m % n;
  }
  return n;
}

int main(int argc, char* argv[])
{
  if (3 != argc)
  {
    fprintf(stderr, "usage: gcd [NUMBER] [NUMBER]\n");
    return 1;
  }

  unsigned m{0};
  if (sscanf(argv[1], "%u", &m) != 1)
  {
    fprintf(stderr, "error while parsing first argument\n");
    return 1;
  }

  unsigned n{0};
  if (sscanf(argv[2], "%u", &n) != 1)
  {
    fprintf(stderr, "error while parsing second argument\n");
    return 1;
  }

  printf("GCD of %u and %u is %u\n", m, n, gcd(m, n));

  return 0;
}
