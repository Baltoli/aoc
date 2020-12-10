#include <utils/utils.h>

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> get_input() { return utils::map_lines(utils::stoi); }

int part_1(std::vector<int> in)
{
  std::sort(in.begin(), in.end());

  auto ones   = 1;
  auto threes = 1;

  for (auto i = 0; i < in.size() - 1; ++i) {
    auto diff = in[i + 1] - in[i];

    ones += (diff == 1);
    threes += (diff == 3);
  }

  return ones * threes;
}

int part_2(std::vector<int> in)
{
  /*
   * Flip the arrows and walk the graph backwards - paths(N) = sum_(s in suc(N))
   * paths(s)
   *
   * Memoize a table to avoid recursive blowup
   */
  return 0;
}

int main()
{
  auto in = get_input();
  std::cout << part_1(in) << '\n';
}
