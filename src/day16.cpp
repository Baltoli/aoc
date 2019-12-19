#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> read_input()
{
  std::string line;
  std::getline(std::cin, line);

  auto ret = std::vector<int> {};

  std::transform(line.begin(), line.end(), std::back_inserter(ret), [](auto c) {
    return c - '0';
  });

  return ret;
}

std::vector<std::vector<int>> patterns(int max_len)
{
  auto base_pattern = std::array {0, 1, 0, -1};

  auto ret = std::vector<std::vector<int>> {};
  ret.reserve(max_len);

  for (int digit = 0; digit < max_len; ++digit) {
    auto pattern = std::vector<int> {};
    pattern.reserve(max_len);

    for (int value = 0; value < max_len + 1; ++value) {
      auto base = (value / (digit + 1)) % base_pattern.size();
      if (value > 0) {
        pattern.push_back(base_pattern[base]);
      }
    }

    ret.push_back(pattern);
  }

  return ret;
}

int main()
{
  auto in   = read_input();
  auto pats = patterns(in.size());
}
