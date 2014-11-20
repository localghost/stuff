#include <iostream>
#include <unordered_set>
#include <fstream>
#include <set>

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

  std::unordered_set<long> ht(10000);
	std::set<long> hht;
	std::set<long> res;
  std::unordered_set<long> t(1000);
  long counter = 10000;

  long cur;
  while (f >> cur)
  {
		hht.insert(cur);
	}
	f.close();
	std::cout << hht.size() << std::endl;

	int dummy;
	for (auto it = hht.begin(); it != hht.end(); ++it)
	{
		--counter;
		if (counter == 0) { std::cout << "+" << res.size() << std::endl; counter = 10000; }
		auto lower = hht.lower_bound(-(*it + 10000));
		auto upper = hht.upper_bound(-(*it - 10000));
		if (lower == hht.end()) continue;
		for (auto it2 = lower; it2 != upper; ++it2)
		{
			if (*it == *it2) continue;
			res.insert(*it + *it2);
		}
	}

  std::cout << res.size() << std::endl;
}
