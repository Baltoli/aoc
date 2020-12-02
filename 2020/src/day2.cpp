#include <utils/utils.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

bool valid_p1(std::string const& pass, char c, int min, int max)
{
  auto count = std::count(pass.begin(), pass.end(), c);
  return count >= min && count <= max;
}

bool valid_p2(std::string const& pass, char c, int min, int max)
{
  auto total = (pass[min] == c) + (pass[max] == c);
  return total == 1;
}

int main()
{
  int n_p1 = 0;
  int n_p2 = 0;

  utils::for_each_line([&](auto const& line) {
    auto sp   = utils::split(line, ":");
    auto pass = sp[1];

    auto pat   = utils::split(sp[0], " ");
    auto range = utils::split(pat[0], "-");
    auto chr   = pat[1][0];

    assert(pat[1].size() == 1);

    auto min = std::stoi(range[0]);
    auto max = std::stoi(range[1]);

    assert(min <= max);

    n_p1 += valid_p1(pass, chr, min, max);
    n_p2 += valid_p2(pass, chr, min, max);
  });

  std::cout << n_p1 << '\n';
  std::cout << n_p2 << '\n';
}
