#include <utils/utils.h>

#include <cstdlib>
#include <iostream>
#include <set>

int main()
{
  auto reqs = std::set<int> {};

  utils::for_each_line([&](auto const& line) {
    auto val = std::atoi(line.c_str());

    if (reqs.find(val) != reqs.end()) {
      std::cout << (val * (2020 - val)) << '\n';
      return;
    }

    reqs.insert(2020 - val);
  });
}
