#include <utils/utils.h>

#include <iomanip>
#include <iostream>
#include <vector>

std::vector<int> get_input()
{
  return utils::map_lines([](auto const& line) { return std::stoi(line); });
}

std::pair<int, int>
combinations(int total, std::vector<int> const& nums, int max = 0)
{
  auto width  = total + 1;
  auto height = nums.size() + 1;

  auto matrix = std::vector<std::pair<int, int>>(width * height, {0, 0});

  auto at = [&](auto nth, auto sum) -> auto&
  {
    return matrix.at(width * nth + sum);
  };

  for (auto first_n = 1; first_n < height; ++first_n) {
    auto new_num = nums.at(first_n - 1);

    for (auto sum = 1; sum < width; ++sum) {
      at(first_n, sum).first
          = at(first_n - 1, sum).first
            + (sum - new_num >= 0 ? at(first_n - 1, sum - new_num).first : 0);

      at(first_n, sum).second = std::min(
          (at(first_n - 1, sum).first > 0 ? at(first_n - 1, sum).second
                                          : total),
          +(sum - new_num >= 0 ? at(first_n - 1, sum - new_num).second + 1
                               : total));
    }

    at(first_n, new_num).first += 1;
    at(first_n, new_num).second = 1;

    for (auto sum = 1; sum < width; ++sum) {
      if (max > 0 && at(first_n, sum).second > max) {
        at(first_n, sum) = {0, 0};
      }
    }
  }

  return at(nums.size(), total);
}

int combs_using(int total, std::vector<int> const& nums)
{
  int t = 0;

  for (auto i = 0; i < nums.size(); ++i) {
    for (auto j = i + 1; j < nums.size(); ++j) {
      for (auto k = j + 1; k < nums.size(); ++k) {
        for (auto l = k + 1; l < nums.size(); ++l) {
          if (nums.at(i) + nums.at(j) + nums.at(k) + nums.at(l) == total) {
            ++t;
          }
        }
      }
    }
  }

  return t;
}

int main()
{
  auto nums       = get_input();
  auto [combs, n] = combinations(150, nums);
  std::cout << combs << '\n';
  std::cout << combs_using(150, nums) << '\n';
}
