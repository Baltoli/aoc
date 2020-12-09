#include <utils/utils.h>

#include <iostream>
#include <set>
#include <vector>

std::vector<long> get_input() { return utils::map_lines(utils::stol); }

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

long contiguous_sum(long target, std::vector<long> const& nums)
{
  auto low  = 0;
  auto high = 1;
  auto sum  = nums[low] + nums[high];

  while (sum < target) {
    auto test = sum + nums[high + 1];

    if (test <= target) {
      sum += nums[++high];
    } else {
      sum -= nums[low++];
    }
  }

  auto b = nums.begin() + low;
  auto e = nums.begin() + high;
  return *std::min_element(b, e) + *std::max_element(b, e);
}

int main()
{
  auto in    = get_input();
  auto sum   = first_non_sum(25, in);
  auto r_sum = contiguous_sum(sum, in);

  std::cout << sum << '\n';
  std::cout << r_sum << '\n';
}
