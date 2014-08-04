#ifndef RANDOM_SEQUENCE_H_
#define RANDOM_SEQUENCE_H_

#include <random>

#define IntType typename
#define OutputIterator typename

/// Returns, via `output` iterator, sequence of `n` integral values with uniform
/// distribution.
template<IntType T = int, OutputIterator O>
O random_sequence(T from, T to, unsigned long n, O output)
{
  std::random_device device;
  std::mt19937 generator{device()};
  std::uniform_int_distribution<T> distribution{from, to};
  for (unsigned long i = 0; i < n; ++i)
    *output++ = distribution(generator);
  return output;
}

#endif
