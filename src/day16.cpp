#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

using lookup_table = std::vector<std::vector<int>>;

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

int ones(int in) { return std::abs(in % 10); }

lookup_table patterns(int max_len)
{
  auto base_pattern = std::array {0, 1, 0, -1};

  auto ret = lookup_table {};
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

std::vector<int>
fft_phase(std::vector<int> const& data, lookup_table const& lut)
{
  assert(data.size() == lut.size() && "Lookup is the wrong size");
  auto ret = std::vector<int>(data);

  for (int i = 0; i < data.size(); ++i) {
    auto const& pat = lut[i];
    auto        sum = 0;

    for (int j = 0; j < data.size(); ++j) {
      sum += ret[j] * pat[j];
    }

    ret[i] = ones(sum);
  }

  return ret;
}

int main()
{
  /* auto in   = read_input(); */
  /* auto pats = patterns(in.size()); */

  /* for (int i = 0; i < 100; ++i) { */
  /*   in = fft_phase(in, pats); */
  /* } */

  /* for (int i = 0; i < 8; ++i) { */
  /*   std::cout << in[i]; */
  /* } */
  /* std::cout << '\n'; */
}
