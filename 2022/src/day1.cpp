#include <utils/utils.h>

#include <iostream>
#include <numeric>

long part_1(std::vector<long> const& totals) { return totals[0]; }

long part_2(std::vector<long> const& totals)
{
  return std::accumulate(totals.begin(), totals.begin() + 3, 0);
}

int main()
{
  auto totals  = std::vector<long> {};
  auto current = 0L;

  utils::for_each_line([&](auto& line) {
    if (line.empty()) {
      totals.push_back(current);
      current = 0;
    } else {
      current += utils::svtol(line);
    }
  });

  std::sort(totals.rbegin(), totals.rend());

  std::cout << part_1(totals) << '\n';
  std::cout << part_2(totals) << '\n';
}
