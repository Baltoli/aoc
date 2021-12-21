#include <utils/utils.h>

#include <iostream>
#include <numeric>
#include <set>

int part_1(std::vector<int> const& in)
{
  return std::accumulate(
      in.begin(), in.end(), 0, [](auto acc, auto i) { return acc + i; });
}

int part_2(std::vector<int> const& in)
{
  auto visited = std::set<int> {};
  auto total   = 0;

  while (true) {
    for (auto i : in) {
      auto [it, ins] = visited.insert(total);
      if (!ins) {
        return total;
      }

      total += i;
    }
  }
}

int main()
{
  auto in
      = utils::map_lines([](auto const& line) { return utils::stoi(line); });

  std::cout << part_1(in) << '\n';
  std::cout << part_2(in) << '\n';
}
