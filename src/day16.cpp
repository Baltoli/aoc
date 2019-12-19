#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

using lookup_table = std::vector<std::vector<int>>;

std::vector<int> read_input(std::string const& line, int times)
{
  auto ret = std::vector<int> {};

  for (int i = 0; i < times; ++i) {
    std::transform(
        line.begin(), line.end(), std::back_inserter(ret),
        [](auto c) { return c - '0'; });
  }

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

void fft_phase(std::vector<int>& data)
{
  auto base_pattern = std::array {0, 1, 0, -1};

  auto max_len = data.size();
  for (int digit = 0; digit < max_len; ++digit) {
    auto sum  = 0;
    auto base = 1;
    auto skip = false;

    for (auto group = digit; group < max_len; group += (digit + 1)) {
      if (!skip) {
        for (auto i = group; i < group + (digit + 1) && i < max_len; ++i) {
          sum += data[i] * base;
        }

        base = -base;
      }

      skip = !skip;
    }

    data[digit] = ones(sum);
  }

  /* for (int i = 0; i < data.size(); ++i) { */
  /*   auto const& pat = lut[i]; */
  /*   auto        sum = 0; */

  /*   for (int j = 0; j < data.size(); ++j) { */
  /*     sum += ret[j] * pat[j]; */
  /*   } */

  /*   ret[i] = ones(sum); */
  /* } */
}

void part_1(std::string const& line)
{
  auto in = read_input(line, 1);

  for (int i = 0; i < 100; ++i) {
    fft_phase(in);
  }

  for (int i = 0; i < 8; ++i) {
    std::cout << in[i];
  }
  std::cout << '\n';
}

int get_offset(std::vector<int> const& m)
{
  return m[0] * 1000000 + m[1] * 100000 + m[2] * 10000 + m[3] * 1000
         + m[4] * 100 + m[5] * 10 + m[6];
}

void part_2(std::string const& line)
{
  auto in = read_input(line, 10000);

  for (int i = 0; i < 100; ++i) {
    std::cout << "Phase " << i << '\n';
    fft_phase(in);
  }

  auto off = get_offset(in);

  for (int i = off; i < off + 8; ++i) {
    std::cout << in[i];
  }
  std::cout << '\n';
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  part_1(line);
  part_2(line);
}
