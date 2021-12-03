#include <utils/utils.h>

#include <iostream>
#include <map>

std::map<int, int> parity(std::vector<std::string> const& lines)
{
  auto data = std::map<int, int> {};

  for (auto const& line : lines) {
    for (auto i = 0; i < line.size(); ++i) {
      auto idx = line.size() - i - 1;

      data.try_emplace(i, 0);

      data[i] += line[idx] == '1' ? 1 : -1;
    }
  }

  return data;
}

int part_1(std::vector<std::string> const& lines)
{
  auto data = parity(lines);

  auto gamma   = 0;
  auto epsilon = 0;

  for (auto const& [k, v] : data) {
    if (v > 0) {
      gamma += (1 << k);
    } else {
      epsilon += (1 << k);
    }
  }

  return gamma * epsilon;
}

int binary_decode(std::string const& s)
{
  int v = 0;
  for (auto i = 0; i < s.size(); ++i) {
    v += (s[s.size() - i - 1] - 48) << i;
  }
  return v;
}

int part_2(std::vector<std::string> lines)
{
  auto co2    = lines;
  auto oxygen = lines;

  for (int i = 0; oxygen.size() > 1 && i < oxygen[0].size(); ++i) {
    auto idx = oxygen[0].size() - i - 1;
    auto p   = parity(oxygen);

    std::erase_if(oxygen, [&](auto const& line) {
      return p[idx] >= 0 ? line[i] == '0' : line[i] == '1';
    });
  }

  for (int i = 0; co2.size() > 1 && i < co2[0].size(); ++i) {
    auto idx = co2[0].size() - i - 1;
    auto p   = parity(co2);

    std::erase_if(co2, [&](auto const& line) {
      return p[idx] >= 0 ? line[i] == '1' : line[i] == '0';
    });
  }

  return binary_decode(co2[0]) * binary_decode(oxygen[0]);
}

int main()
{
  auto input = utils::get_lines();

  std::cout << part_1(input) << '\n';
  std::cout << part_2(input) << '\n';
}
