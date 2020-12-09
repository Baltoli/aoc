#include <utils/utils.h>

#include <iostream>
#include <set>
#include <vector>

std::vector<long> get_input()
{
  return utils::map_lines([](auto const& line) { return std::stol(line); });
}

long first_non_sum(int window, std::vector<long> const& nums)
{
  auto pairs = std::vector<std::pair<long, long>> {};
  for (auto i = 0; i < window; ++i) {
    for (auto j = 0; j < window; ++j) {
      if (i != j) {
        pairs.push_back({nums[i], nums[j]});
      }
    }
  }

  for (auto i = window; i < nums.size(); ++i) {
    bool any = false;

    for (auto const& [a, b] : pairs) {
      if (nums[i] == a + b) {
        any = true;
        break;
      }
    }

    if (!any) {
      return nums[i];
    }

    for (auto& p : pairs) {
      auto leave = nums[i - window];

      if (p.first == leave) {
        p.first = nums[i];
        continue;
      }

      if (p.second == leave) {
        p.second = nums[i];
        continue;
      }
    }
  }

  return 0;
}

int main()
{
  auto in = get_input();
  std::cout << first_non_sum(25, in) << '\n';
}
