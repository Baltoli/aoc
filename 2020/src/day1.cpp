#include <utils/utils.h>

#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <utility>
#include <vector>

std::vector<int> get_input()
{
  auto ret = std::vector<int> {};
  utils::for_each_line(
      [&ret](auto const& line) { ret.push_back(std::atoi(line.c_str())); });
  return ret;
}

std::optional<std::pair<int, int>>
pair_sum_to(std::vector<int> const& cont, int sum, int exclude = 0)
{
  auto reqs = std::set<int> {};

  for (auto val : cont) {
    if (val == exclude) {
      return std::nullopt;
    }

    if (reqs.find(val) != reqs.end()) {
      return std::pair {val, sum - val};
    }

    reqs.insert(sum - val);
  }

  return std::nullopt;
}

void part_1(std::vector<int> const& in)
{
  if (auto p = pair_sum_to(in, 2020)) {
    std::cout << (p->first * p->second) << '\n';
  }
}

void part_2(std::vector<int> const& in)
{
  for (auto val : in) {
    auto remain = 2020 - val;

    if (auto p = pair_sum_to(in, remain, val)) {
      std::cout << (val * p->first * p->second) << '\n';
    }
  }
}

int main()
{
  auto in = get_input();

  part_1(in);
  part_2(in);
}
