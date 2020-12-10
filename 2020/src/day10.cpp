#include <utils/utils.h>

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

std::vector<long> get_input() { return utils::map_lines(utils::stol); }

long part_1(std::vector<long> in)
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

long part_2_helper(
    long idx, std::vector<long> const& in,
    std::unordered_map<long, long>& paths)
{
  if (paths.find(in[idx]) == paths.end()) {
    auto sum = 0l;

    for (auto off = 1; off < 4 && idx + off < in.size(); ++off) {
      auto next = in[idx + off];
      if (in[idx] - next <= 3) {
        sum += part_2_helper(idx + off, in, paths);
      }
    }

    paths[in[idx]] = sum;
  }

  return paths.at(in[idx]);
}

long part_2(std::vector<long> in)
{
  auto paths = std::unordered_map<long, long> {};

  std::sort(in.begin(), in.end());
  in.push_back(in[in.size() - 1] + 3);

  std::reverse(in.begin(), in.end());
  in.push_back(1);

  paths[1] = 1;
  return part_2_helper(0, in, paths);
}

int main()
{
  auto in = get_input();
  std::cout << part_1(in) << '\n';
  std::cout << part_2(in) << '\n';
}
