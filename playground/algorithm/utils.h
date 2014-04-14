#ifndef UTILS_H
#define UTILS_H

#include <string>

template<typename I>
void print_range(I first, I last, const std::string& sep = " ")
{
  for (auto it = first; it != last; ++it)
    std::cout << *it << sep;
  std::cout << std::endl;
}

#endif
