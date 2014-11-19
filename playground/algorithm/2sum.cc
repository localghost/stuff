#include <iostream>
#include <unordered_set>
#include <fstream>

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "data file is missing" << std::endl;
    return 1;
  }

  std::fstream f{argv[1]};
  if (!f)
  {
    std::cerr << "could not open data file" << std::endl;
    return 2;
  }

  std::unordered_set<long> ht(100000);
  std::unordered_set<long> t(1000);
  long counter = 10000;

  long cur;
  while (f >> cur)
  {
    --counter;
    if (counter == 0) { std::cout << "+" << t.size() << std::endl; counter = 10000; }
    if (ht.find(cur) != ht.end()) continue;

      for (long it = -(cur + 10000); it <= -(cur - 10000); ++it)
      {
        if (t.find(it+cur) != t.end()) continue;
        const auto found = ht.find(it);
        if (found != ht.end())
          t.insert(it+cur);
      }
    ht.insert(cur);
  }
  f.close();

  std::cout << t.size() << std::endl;
}
